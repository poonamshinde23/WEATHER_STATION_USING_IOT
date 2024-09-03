from flask import Flask, request
from my_db import execute_select_query
from my_db import execute_query
from my_response import create_response

app = Flask(__name__)

@app.route("/readingdata", methods=["GET"])
def get_allsensors():
    query = "select time, temperature, humidity,ldrValue, RainDetected from allsensors;"

    allsensors = execute_select_query(query)

    list_allsensors = []
    for (time ,temperature, humidity, ldrValue, RainDetected ) in allsensors:
        list_allsensors.append({
            #"time":time,
            "temperature":temperature,
            "humidity":humidity,
            "ldrValue":ldrValue,
            "RainDetected":RainDetected,
            "time":time
            
        })

    print(list_allsensors)
    return create_response(list_allsensors)
#-------------------------------------------------------------

@app.route("/readingdata", methods=["POST"])
def insert_allsensors():
	
    temperature = request.get_json().get('temperature')
    humidity = request.get_json().get('humidity')
    ldrValue = request.get_json().get('ldrValue')
    RainDetected = request.get_json().get('RainDetected')
    
    query = f"insert into allsensors (temperature, humidity, ldrValue, RainDetected) values ({temperature}, {humidity}, {ldrValue}, {RainDetected});"

    execute_query(query)

    return create_response("allsensors added successfully")



#------------------------------------------------------------------

app.run(host="0.0.0.0", port=5000, debug=True)
