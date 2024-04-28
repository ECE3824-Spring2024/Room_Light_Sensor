import mysql.connector
import configparser

def connect():
    config = configparser.ConfigParser()
    config.read("../flaskconfig.cfg")
    host = config['FLASK']['host']
    user = config['FLASK']['user']
    password = config['FLASK']['password']
    database = config['FLASK']['database']
    try:
        # Establish connection to the MySQL database
        connection = mysql.connector.connect(
            host=host,
            user=user,
            password=password,
            database=database
        )

        # Check if the connection is successful
        if connection.is_connected():
            print("Successfully connected to the MySQL database.")
            return connection

    except mysql.connector.Error as error:
        print("Error:", error)


def ping_mysql_db(connection):
   
    # Ping the database to check if it's reachable
    cursor = connection.cursor()
    cursor.execute("SELECT 1")
    result = cursor.fetchone()

    # Print the result
    print("Ping result:", result[0])
    cursor.close()


def search_by_date(connection, date):
    cursor = connection.cursor()
    cursor.execute(f"SELECT * from hourly_lights where date_entry = {date}")
    result = cursor.fetchall()

    cursor.close()

    return result

def get_sum(input):
    sum = 0
    for i, item in enumerate(input[0]):
        if i == 0 or i == 1:
            pass
        else:
            sum += int(item)

    print(f"Sum is {sum}")
    return sum

def check_valid(connection, date=None, room=None):
    cursor = connection.cursor()
    if date is not None and room is None:
        cursor.execute(f"SELECT EXISTS(SELECT 1 FROM hourly_lights WHERE date_entry = {date})")
        result = cursor.fetchone()
        print("1")
    elif date is None and room is not None:
        cursor.execute(f"SELECT EXISTS(SELECT 1 FROM hourly_lights WHERE room = {room})")
        result = cursor.fetchone()
        print("2")
    elif date is not None and room is not None:
        cursor.execute(f"SELECT EXISTS(SELECT 1 FROM hourly_lights WHERE date_entry = {date} AND room = {room})")
        result = cursor.fetchone()
        print("3")
        
    cursor.close()

    if result[0] == 1:
        print("True")
        return True
    else:
        print("False")
        return False
    
def create_new_row(connection, date, room):
    print("Creating new room")
    cursor = connection.cursor()
    cursor.execute(f"INSERT INTO hourly_lights VALUES ({room}, {date}, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)")
    
    connection.commit()
    cursor.close()
    
    return True
    
def increment_hour(connection, date, room, hour):
    cursor = connection.cursor()
    cursor.execute(f"UPDATE hourly_lights SET {hour} = {hour} + 1 WHERE room = {room} AND date_entry = {date}")
    
    connection.commit()
    cursor.close()
    


def main():

    connection = connect()
    ping_mysql_db(connection)
    print(check_valid(connection, "'2024-04-10'"))
    output = search_by_date(connection, "'2024-04-10'")
    sum_result = get_sum(output)

    connection.close()


if __name__ == '__main__':
    main()
