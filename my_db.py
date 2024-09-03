# this is a module to handle all database operations
import mysql.connector

def open_connection():
    connection = mysql.connector.connect( 
        host="localhost",
        port=3306,
        user="sunbeam",
        password="sunbeam",
        database="project"
    )
    return connection

def execute_query(query):
    connection = open_connection()

    cursor = connection.cursor()
    cursor.execute(query)
    connection.commit()

    cursor.close()
    connection.close()

def execute_select_query(query):
    connection = open_connection()

    cursor = connection.cursor()
    cursor.execute(query)
    data = cursor.fetchall()

    cursor.close()
    connection.close()

    return data
