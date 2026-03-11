from flask import Flask, request, jsonify
import subprocess
import tempfile
import os

app = Flask(__name__)


def run_parser(command, json_text, extra_args=None):

    if extra_args is None:
        extra_args = []

    # create temporary JSON file
    with tempfile.NamedTemporaryFile(delete=False, suffix=".json", mode="w") as f:
        f.write(json_text)
        filepath = f.name

    try:
        result = subprocess.run(
            ["./json_parser", command, filepath] + extra_args,
            capture_output=True,
            text=True
        )

        return result.stdout, result.stderr

    finally:
        os.remove(filepath)


@app.route("/validate", methods=["POST"])
def validate():

    json_text = request.get_data(as_text=True)

    out, err = run_parser("validate", json_text)

    return jsonify({
        "result": out.strip(),
        "error": err.strip()
    })


@app.route("/pretty", methods=["POST"])
def pretty():

    json_text = request.get_data(as_text=True)

    out, err = run_parser("pretty", json_text, ["stdout"])

    return jsonify({
        "result": out.strip(),
        "error": err.strip()
    })


@app.route("/minify", methods=["POST"])
def minify():

    json_text = request.get_data(as_text=True)

    out, err = run_parser("minify", json_text, ["stdout"])

    return jsonify({
        "result": out.strip(),
        "error": err.strip()
    })


@app.route("/show", methods=["POST"])
def show():

    json_text = request.get_data(as_text=True)

    out, err = run_parser("show", json_text)

    return jsonify({
        "result": out.strip(),
        "error": err.strip()
    })


@app.route("/get", methods=["POST"])
def get_value():

    data = request.json

    json_text = data["json"]
    path = data["path"]

    out, err = run_parser("get", json_text, [path])

    return jsonify({
        "result": out.strip(),
        "error": err.strip()
    })


@app.route("/set", methods=["POST"])
def set_value():

    data = request.json

    json_text = data["json"]
    path = data["path"]
    value = data["value"]

    out, err = run_parser("set", json_text, [path, value, "stdout"])

    return jsonify({
        "result": out.strip(),
        "error": err.strip()
    })


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)