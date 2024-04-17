#!/usr/bin/env python

from flask import Flask, render_template, request
 
app = Flask("myapp")
 

@app.route("/date")
def date():
    mydate = request.args.get('input_date')
    print(mydate)
    return render_template("date.html",time_on=mydate)

@app.route("/")
def index():
    return render_template("base.html")
 
 
if __name__ == "__main__":
    app.run(debug=True)
