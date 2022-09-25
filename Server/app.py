import json
import time

from flask import Flask, Response, render_template, stream_with_context
from db.db import fetch_data


app = Flask(__name__)


@app.route('/')
def index():
    return render_template("index.html")


@app.route('/chart-data')
def chart_data():
    query = """SELECT temperature, time FROM Temperature Order by date DESC, time DESC LIMIT 1;"""

    def load_data():
        while True:
            data = fetch_data('db/Temperature', query)
            value = data[0][0]
            t = data[0][1]

            json_data = json.dumps({'time': t,
                                   'value': value})
            yield f'data:{json_data}\n\n'
            time.sleep(0.1)

    response = Response(stream_with_context(load_data()), mimetype="text/event-stream")
    response.headers["Cache-Control"] = "no-cache"
    response.headers["X-Accel-Buffering"] = "no"
    return response


if __name__ == "__main__":
    app.run(debug=True, threaded=True)
