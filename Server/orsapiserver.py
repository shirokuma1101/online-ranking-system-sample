# standard
import datetime
import json
import os
import sqlite3
import urllib.parse
from wsgiref.simple_server import make_server


# online ranking system database
class ORSDB:

    # public

    # constants
    DB_NAME = 'ors.db'
    TABLE_NAME = 'ors'
    KEY_LIST = ['log_time', 'uuid', 'user_name', 'score']
    # queries
    CREATE_NEW_TABLE       = f'CREATE TABLE {TABLE_NAME}(log_time TEXT, uuid TEXT, user_name TEXT, score INTEGER)'
    INSERT_NEW_SCORE       = f'INSERT INTO {TABLE_NAME}(log_time, uuid, user_name, score) VALUES (?, ?, ?, ?)'
    UPDATE_SCORE           = f'UPDATE {TABLE_NAME} SET log_time = (?), score = (?) WHERE uuid = (?)'
    SEARCH_BY_UUID         = f'SELECT * FROM {TABLE_NAME} WHERE uuid = (?)'
    COMPARE_SCORES_BY_UUID = f'SELECT * FROM {TABLE_NAME} WHERE score <= (?) AND uuid = (?)'
    TOP_RANKING            = f'SELECT * FROM {TABLE_NAME} ORDER BY score DESC LIMIT (?)'
    MY_RANKING             = f'SELECT * FROM(SELECT *, RANK() OVER(ORDER BY score DESC) AS ranking FROM {TABLE_NAME}) WHERE uuid = (?)'

    def write_new_score(self, uuid: str, user_name: str, score: int) -> None:
        # get log time
        log_time = self._get_log_time()
        # check if uuid exists
        if (self._execute(self.SEARCH_BY_UUID, [uuid])):
            # check if score is higher than the previous one
            if (self._execute(self.COMPARE_SCORES_BY_UUID, [score, uuid])):
                # update score
                self._execute(self.UPDATE_SCORE, [log_time, score, uuid])
        else:
            # insert new score
            self._execute(self.INSERT_NEW_SCORE, [log_time, uuid, user_name, score])

    def get_top_ranking(self, limit: int) -> dict:
        # get ranking
        ranking = self._execute(self.TOP_RANKING, [limit])

        # convert list to dict
        ## (log_time, uuid, user_name, score) -> {ranking: {log_time, uuid, user_name, score}}
        if ranking:
            sorted_ranking = {}
            for i, e in enumerate(ranking, 1):
                sorted_ranking[i] = dict(zip(self.KEY_LIST, e))
            return sorted_ranking

        return {}

    def get_my_ranking(self, uuid: str) -> dict:
        ranking = self._execute(self.MY_RANKING, [uuid])

        # convert list to dict
        ## (log_time, uuid, user_name, score, ranking) -> {ranking: {log_time, uuid, user_name, score}}
        if ranking:
            return {str(ranking[0][-1]): dict(zip(self.KEY_LIST, ranking[0]))}

        return {}

    def reset_ranking(self) -> None:
        # delete database file
        try:
            os.remove(self.DB_NAME)
        except FileNotFoundError:
            pass
        # create new table
        self._execute(self.CREATE_NEW_TABLE)

    # private

    def _execute(self, query, params=()) -> list:
        with sqlite3.connect(self.DB_NAME) as conn:
            cur = conn.cursor()
            cur.execute(query, params)
            conn.commit()
            res = cur.fetchall()

        return res

    def _get_log_time(self) -> str:
        return datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')


# online ranking system api server
class ORSAPIServer:

    # public

    def __init__(self, orsdb: ORSDB, host: str = 'localhost', port: int = 5000):
        self.orsdb = orsdb
        self.host = host
        self.port = port

    def start(self) -> None:
        with make_server(self.host, self.port, self._app) as httpd:
            print(f'Serving on {self.host}:{self.port}...')
            httpd.serve_forever()

    # private

    def _app(self, environ, response) -> list:

        header = [
            ('Access-Control-Allow-Origin', '*'),
            ('Access-Control-Allow-Headers', 'Content-Type'),
            ('Access-Control-Allow-Methods', 'GET, POST'),
        ]

        request_method = environ.get('REQUEST_METHOD')

        # GET
        if request_method == 'GET':
            query_string = environ.get('QUERY_STRING')
            if query_string:
                # parse query string
                qs = urllib.parse.parse_qs(query_string)

                # get my ranking
                uuid = qs.get('uuid')
                if uuid:
                    res = self.orsdb.get_my_ranking(uuid[0])

                # get top ranking
                limit = qs.get('limit')
                if limit:
                    # get top ranking
                    res = self.orsdb.get_top_ranking(int(limit[0]))
            else:
                # get all ranking
                res = self.orsdb.get_top_ranking(-1)

            # convert dict to json
            res = json.dumps(res).encode('utf-8')
            # set header
            header.append(('Content-Type', 'application/json; charset=utf-8'))
            header.append(('Content-Length', str(len(res))))
            # set status
            status = '200 OK'

            # send response
            response(status, header)
            return [res]

        if request_method == 'POST':
            # get request body
            wsgi_input = environ.get('wsgi.input')
            if wsgi_input is None:
                response('400 Bad Request', header)
                return []

            # parse request body
            req = json.loads(wsgi_input.read(int(environ.get('CONTENT_LENGTH', 0))).decode('utf-8'))
            if req:
                uuid = req.get('uuid')
                user_name = req.get('user_name')
                score = req.get('score')
                if uuid and user_name and score:
                    # write new score
                    self.orsdb.write_new_score(uuid, user_name, score)
                    response('200 OK', header)
                    return []

            response('400 Bad Request', header)
            return []


def main():

    db = ORSDB()
    db.reset_ranking()
    ors_api_server = ORSAPIServer(db, '192.168.1.15')
    ors_api_server.start()


if __name__ == '__main__':
    main()

