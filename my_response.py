# this is module to create response

from flask import jsonify

def create_response(data, error=None):

    d = dict()

    if error == None:
        d = {
            "status":"Success",
            "data":data
        }
    else:
        d = {"status":"Failure"}

    return jsonify(d)
