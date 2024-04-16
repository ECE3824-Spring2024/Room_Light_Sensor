#!/usr/bin/env python

from flask import Flask, render_template
 
app = Flask("myapp")
 

@app.route("/date")
def date():
    return render_template("date.html",title="Light Usage For 4/7/2003",content="3 Hrs On")

@app.route("/")
def index():
    return render_template(
        "base.html", title="Welcome", content="Welcome to our website"
    )
 
 
if __name__ == "__main__":
    app.run(debug=True)
