import sys
from flask import Flask, render_template, request
from utils.dbtools import connect, ping_mysql_db, get_sum, search_by_date, check_valid

app = Flask("myapp")
 
@app.route("/date")
def date():
    mydate = request.args.get('input_date')
    mydate_formatted = f"'{mydate}'"
    connection = connect()
    if check_valid(connection, mydate_formatted):
        result = search_by_date(connection, mydate_formatted)
        result = get_sum(result)
    else:
        result = "N/A"
    connection.close()
    return render_template("date.html",date=mydate, time_on=result)

@app.route("/")
def index():
    return render_template("base.html")
 
 
if __name__ == "__main__":

    app.run(debug=True)
