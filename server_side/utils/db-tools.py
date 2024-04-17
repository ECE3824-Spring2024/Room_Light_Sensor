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

    print(result)

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

def main():

    connection = connect()
    ping_mysql_db(connection)
    output = search_by_date(connection, "'2024-04-10'")
    sum_result = get_sum(output)

    connection.close()


if __name__ == '__main__':
    main()
