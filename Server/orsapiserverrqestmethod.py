# standard
import json
import uuid

# request
import requests

# debug
from pprint import pprint


# constants
DEFAULT_URL = 'http://192.168.1.15:5000'


def request(url, method, params=None) -> dict:
    if method == 'GET':
        return json.loads(requests.get(url).text)
    elif method == 'POST':
        return requests.post(url, json=params)


def make_request() -> dict:
    # get url
    print(f'if empty, use default url. ({DEFAULT_URL})')
    url = input('Input url: ')
    if not url:
        url = DEFAULT_URL

    # get request method
    print('1. GET')
    print('2. POST')
    print('if empty, use default method. (GET)')
    method = input('Select request method: ')
    if method == '1' or not method:
        method = 'GET'
    elif method == '2':
        method = 'POST'
    else:
        raise ValueError('Invalid request method')

    # get params
    params = None

    if method == 'GET':

        print('if empty, skip.')
        _uuid = input('Input uuid: ')
        if _uuid:
            url += f'?uuid={_uuid}'
        else:
            print('if empty, skip.')
            limit = input('Input limit: ')
            if limit:
                url += f'?limit={str(limit)}'

    elif method == 'POST':
        _uuid = str(uuid.uuid4())
        user_name = input('Input user_name: ')
        score = input('Input score: ')
        params = {
            'uuid': _uuid,
            'user_name': user_name,
            'score': score
        }

    return {
        'url': url,
        'method': method,
        'params': params
    }


def main():
    pprint(request(**make_request()))


if __name__ == '__main__':
    main()

