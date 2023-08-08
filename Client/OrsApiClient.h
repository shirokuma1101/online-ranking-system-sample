#pragma once

namespace ors_api_client
{
    constexpr char CRLF[] = "\r\n";
    constexpr char CRLFCRLF[] = "\r\n\r\n";

    enum class Method {
        GET,
        POST,
    };

    inline void AddCrlf(std::string* str) {
        str->append(CRLF);
    }

    inline std::tuple<std::string, socket_helper::PORT> SplitUrl(std::string str) {
        if (auto pos = str.find_last_of(":"); pos != std::string::npos) {
            return { str.substr(0, pos).data(), std::stoi(str.substr(pos + 1).data())};
        }
        return { str.data(), 80 };
    }

    inline std::string GetResponseMessageBody(std::string_view response) {
        if (auto crlf_pos = response.find(CRLFCRLF); crlf_pos != std::string::npos) {
            return response.substr(crlf_pos + sizeof(CRLFCRLF) - 1).data();
        }
        return std::string();
    }

    inline json Request(std::string_view url, Method method, const json& params = {}) {

        if (method == Method::GET) {
            // split url into host and port
            auto [host, port] = SplitUrl(url.data());

            // create socket and connect to host
            SOCKET sock = socket_helper::Create();
            ADDRINFO addr_info;
            socket_helper::GetAddrInfo(host, port, &addr_info);
            if (!socket_helper::Connect(&sock, addr_info, 5000)) {
                return json();
            }

            // query string
            std::string query;
            if (params.size()) {
                // start query string
                query.append("?");
                for (const auto& [key, value] : params.items()) {
                    query += std::format("{}={}&", key, value.get<std::string>());
                }
                // remove last &
                query.pop_back();
            }

            // request line
            std::string request_line;
            request_line = std::format("GET {} HTTP/1.1", query);
            AddCrlf(&request_line);

            // header fields
            std::string header_fields;
            header_fields = std::format("Host: {}:{}", host, port);
            AddCrlf(&header_fields);

            // finally send the request
            std::string http_request = request_line + header_fields + CRLF;

            // send request
            socket_helper::Send(sock, http_request);

            // receive response
            int recv_limit = 1024;
            int recv_count = 0;
            std::string response;
            std::string message_body;
            while (++recv_count < recv_limit) {
                int content_length = -1;
                response += socket_helper::Recv(sock);

                // Determine if all data has been received by checking the Content-Length header field
                if (auto pos = response.find("Content-Length: "); pos != std::string::npos) {
                    content_length = std::stoi(response.substr(pos + sizeof("Content-Length: ") - 1).data());
                }
                else if (auto pos = response.find("content-length: "); pos != std::string::npos) {
                    content_length = std::stoi(response.substr(pos + sizeof("content-length: ") - 1).data());
                }

                // if all data has been received, break
                message_body = GetResponseMessageBody(response);
                if (content_length != -1 && message_body.size() == content_length) {
                    break;
                }
            }

            std::string content_type;
            // check if Content-Type is application/json
            if (auto pos = response.find("Content-Type: "); pos != std::string::npos) {
                content_type = response.substr(pos + sizeof("Content-Type: ") - 1);
            }
            else if (auto pos = response.find("content-type: "); pos != std::string::npos) {
                content_type = response.substr(pos + sizeof("content-type: ") - 1);
            }
            if (content_type.find("application/json") == std::string::npos) {
                throw std::exception("Content-Type is not application/json");
            }

            // close socket
            socket_helper::Close(&sock);

            // return message body as json
            return json::parse(message_body);
        }

        else if (method == Method::POST) {
            // split url into host and port
            auto [host, port] = SplitUrl(url.data());

            // create socket and connect to host
            SOCKET sock = socket_helper::Create();
            ADDRINFO addr_info;
            socket_helper::GetAddrInfo(host, port, &addr_info);
            socket_helper::Connect(&sock, addr_info, 5000);

            // request line
            std::string request_line;
            request_line = std::format("POST {} HTTP/1.1", url);
            AddCrlf(&request_line);

            // header fields
            std::string header_fields;
            header_fields = std::format("Host: {}:{}", host, port);
            AddCrlf(&header_fields);
            header_fields += std::format("Content-Type: application/json");
            AddCrlf(&header_fields);
            header_fields += std::format("Content-Length: {}", params.dump().size());
            AddCrlf(&header_fields);

            // finally send the request
            std::string http_request = request_line + header_fields + CRLF + params.dump();

            // send request
            socket_helper::Send(sock, http_request);

            // close socket
            socket_helper::Close(&sock);
        }

        return json();
    }

};
