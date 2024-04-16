import mysql.connector


# Example usage:
host = "lightdb.cvqyyg4i4b64.us-east-2.rds.amazonaws.com"
user = "flask"
password = "flaskpass"
database = "LightDB"

def connect():
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

def main():
    connection = connect()
    ping_mysql_db(connection)
    search_by_date(connection, "'2024-04-10'")

    connection.close()


if __name__ == '__main__':
    main()
