from flask import Flask, render_template, request, jsonify
from utils.dbtools import connect, ping_mysql_db, get_sum, search_by_date, check_valid, create_new_row, increment_hour

app = Flask("myapp")
 
@app.route("/date")
def date():

    # Get argument
    mydate = request.args.get('input_date')
    myroom = request.args.get('room')

    # Format for MySQL
    mydate_formatted = f"'{mydate}'"
    myroom_formatted = f"'{myroom}'"

    # Connect to db
    connection = connect()

    # Check if date is valid before getting total time
    if check_valid(connection, mydate_formatted):
        result = search_by_date(connection, mydate_formatted)
        result = get_sum(result)
    else:
        result = "N/A"

    # Close connection and pass data into separate jinja2 template
    connection.close()
    return render_template("date.html",date=mydate, time_on=result)

@app.route("/increment_column")
def increment_column():
    
    # Get args
    date = request.args.get("date")
    hour = request.args.get("hour")
    room = request.args.get("room")
    
    hour = int(hour)
    room = int(room)
    print(room)
    
    # Format for MySQL
    date = f"'{date}'"
    hour = f"hour{hour}"
    print(hour)
    
    # Connect and see if row exists
    connection = connect()
    
    if check_valid(connection, date, room) is False:
        create_new_row(connection, date, room)
    increment_hour(connection, date, room, hour)
        
    connection.close()
    
    return jsonify(success=True, status_code=200)

@app.route("/")
def index():
    return render_template("base.html")
 
 
if __name__ == "__main__":

    app.run(host='0.0.0.0', port=8080, debug=True)
