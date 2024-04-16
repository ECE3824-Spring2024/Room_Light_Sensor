#!/usr/bin/env python

from flask import Flask, render_template
 
app = Flask("myapp")
 

@app.route("/date")
def date():
    return render_template("date.html")

@app.route("/")
def index():
    return render_template("base.html")
 
 
if __name__ == "__main__":
    app.run(debug=True)
