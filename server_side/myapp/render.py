from flask import Flask, render_template, request, jsonify
from utils.dbtools import connect, ping_mysql_db, get_sum, search_combo, check_valid, create_new_row, increment_hour

app = Flask("myapp")
 
@app.route("/date")
def date():

    # Get argument
    mydate = request.args.get('input_date')
    myroom = request.args.get('room')

    # Format for MySQL
    mydate_formatted = f"'{mydate}'"
    myroom_formatted = int(myroom)

    # Connect to db
    connection = connect()

    # Check if date is valid before getting total time
    if check_valid(connection, mydate_formatted, myroom_formatted):
        result = search_combo(connection, mydate_formatted, myroom_formatted)
        sum_result = get_sum(result)
        connection.close()
        return render_template("date.html",date=mydate, time_on=sum_result, room_number=myroom,
        hour0=result[0][2],
        hour1=result[0][3],
        hour2=result[0][4],
        hour3=result[0][5],
        hour4=result[0][6],
        hour5=result[0][7],
        hour6=result[0][8],
        hour7=result[0][9],
        hour8=result[0][10],
        hour9=result[0][11],
        hour10=result[0][12],
        hour11=result[0][13],
        hour12=result[0][14],
        hour13=result[0][15],
        hour14=result[0][16],
        hour15=result[0][17],
        hour16=result[0][18],
        hour17=result[0][19],
        hour18=result[0][20],
        hour19=result[0][21],
        hour20=result[0][22],
        hour21=result[0][23],
        hour22=result[0][24],
        hour23=result[0][25])
    else:
        result = "N/A"
        connection.close()
        return render_template("date.html",date=mydate, time_on=result, room_number=myroom,
        hour0="N/A",
        hour1="N/A",
        hour2="N/A",
        hour3="N/A",
        hour4="N/A",
        hour5="N/A",
        hour6="N/A",
        hour7="N/A",
        hour8="N/A",
        hour9="N/A",
        hour10="N/A",
        hour11="N/A",
        hour12="N/A",
        hour13="N/A",
        hour14="N/A",
        hour15="N/A",
        hour16="N/A",
        hour17="N/A",
        hour18="N/A",
        hour19="N/A",
        hour20="N/A",
        hour21="N/A",
        hour22="N/A",
        hour23="N/A")
 


@app.route("/increment_column")
def increment_column():
    
    # Get args
    date = request.args.get("date")
    hour = request.args.get("hour")
    room = request.args.get("room")
    
    hour = int(hour)
    room = int(room)
    
    # Format for MySQL
    date = f"'{date}'"
    hour = f"hour{hour}"
    
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
