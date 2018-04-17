#!/usr/bin/env python
# Kyle Fitzsimmons, 2018
from flask import Flask, g
from flask_restful import Resource, Api
import sqlite3
import time

DATABASE = './sensors.sqlite'

app = Flask(__name__)
api = Api(app)


def get_database():
    db = getattr(g, '_database', None)
    if db is None:
        db = g._database = sqlite3.connect(DATABASE)
    return db

@app.teardown_appcontext
def close_connection(exception):
    db = getattr(g, '_database', None)
    if db is not None:
        db.close()

@app.before_first_request
def setup_db():
    sql = '''
        CREATE TABLE IF NOT EXISTS updates(
            id INTEGER PRIMARY KEY,
            timestamp INTEGER
        );
    '''
    db = get_database()
    cur = db.cursor()
    cur.execute(sql)
    db.commit()


class Ping(Resource):
    def get(self):
        return {'message': 0}

    def post(self):
        now = int(time.time())
        sql = '''INSERT INTO updates (timestamp) VALUES (?);'''

        db = get_database()
        cur = db.cursor()
        cur.execute(sql, (now,))
        db.commit()

        return {'message': 1}


api.add_resource(Ping, '/')

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
