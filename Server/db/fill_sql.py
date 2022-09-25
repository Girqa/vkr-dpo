from db import *
from serial_connection import *

def main():
    db_name = 'Temperature'
    create_db(db_name)

    serial = connect_serial()
    try:
        while True:
            text = read_serial_port(serial)
            text_list = prepare_query(text)
            temperature, state, setting = text_list
            insert_data(db_name, temperature, setting, state)
    except:
        print('Error occurred!')
    finally:
        print('Work is stopped.')


if __name__ == '__main__':
    main()