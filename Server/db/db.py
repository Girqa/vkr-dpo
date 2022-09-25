import sqlite3 as sql
import datetime


def commit(db: str, query: str) -> None:
    """Commits query to db
    Args:
        db (str): data base name
        query (str): SQL commands
    """
    try:
        conn = sql.connect(db)
        cursor = conn.cursor()
        cursor.execute(query)
        conn.commit()
        cursor.close()

    except sql.Error as error:
        print("[-] Error while connecting to sqlite", error)
    finally:
        if conn:
            conn.close()


def fetch_data(db: str, query: str):
    try:
        conn = sql.connect(db)
        cursor = conn.cursor()
        cursor.execute(query)
        data = cursor.fetchall()
        cursor.close()
        return data
    except sql.Error as error:
        print("[-] Error while connecting to sqlite", error)
    finally:
        if conn:
            conn.close()


def create_db(db: str):
    query = '''CREATE TABLE IF NOT EXISTS Temperature (
                   id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
                   state CHAR(8) NOT NULL,
                   temperature FLOAT NOT NULL,
                   setting FLOAT NOT NULL,
                   date DATE NOT NULL,
                   time TIME NOT NULL);'''

    commit(db, query)


def insert_data(db: str, temperature: float, setting: float, state: str):
    cur_date = datetime.date.today()
    cur_time = datetime.datetime.now().time()

    values = f'\'{state}\', {temperature}, {setting}, \'{cur_date}\', \'{cur_time}\''
    query = f'INSERT INTO {db}(state, temperature, setting, date, time) VALUES({values})'
    commit(db, query)


def prepare_query(text: str):
    text = text.strip()
    text_list = text.split()
    return text_list
