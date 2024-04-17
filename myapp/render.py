#!/usr/bin/env python

from flask import Flask, render_template, request
 
app = Flask("myapp")
 

@app.route("/date")
def date():
    print(request.args.get('input_date'))
    return render_template("date.html")

@app.route("/")
def index():
    return render_template("base.html")
 
 
if __name__ == "__main__":
    app.run(debug=True)
