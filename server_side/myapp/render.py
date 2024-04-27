from flask import Flask, render_template, request
from utils.dbtools import connect, ping_mysql_db, get_sum, search_by_date, check_valid

app = Flask("myapp")
 
@app.route("/date")
def date():

    # Get argument
    mydate = request.args.get('input_date')

    # Format for MySQL
    mydate_formatted = f"'{mydate}'"

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

@app.route("/")
def index():
    return render_template("base.html")
 
 
if __name__ == "__main__":

    app.run(debug=True)
