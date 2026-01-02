from flask import Flask, render_template, request, jsonify
from flask_socketio import SocketIO
from models import db, Window
import os

app = Flask(__name__, static_url_path='')
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///charlexwebos.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db.init_app(app)
socketio = SocketIO(app)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/windows', methods=['GET'])
def get_windows():
    windows = Window.query.all()
    return jsonify([{
        'id': w.id,
        'window_id': w.window_id,
        'left': w.left,
        'top': w.top,
        'width': w.width,
        'height': w.height,
        'z_index': w.z_index,
        'maximized': w.maximized,
        'minimized': w.minimized,
        'prev_left': w.prev_left,
        'prev_top': w.prev_top,
        'prev_width': w.prev_width,
        'prev_height': w.prev_height
    } for w in windows])

@app.route('/api/windows', methods=['POST'])
def create_window():
    data = request.json
    window = Window(
        window_id=data['window_id'],
        left=data.get('left', 100),
        top=data.get('top', 100),
        width=data.get('width', 400),
        height=data.get('height', 300),
        z_index=data.get('z_index', 1000),
        maximized=data.get('maximized', False),
        minimized=data.get('minimized', False),
        prev_left=data.get('prev_left'),
        prev_top=data.get('prev_top'),
        prev_width=data.get('prev_width'),
        prev_height=data.get('prev_height')
    )
    db.session.add(window)
    db.session.commit()
    return jsonify({'id': window.id}), 201

@app.route('/api/windows/<int:id>', methods=['PUT'])
def update_window(id):
    window = Window.query.get_or_404(id)
    data = request.json
    for key, value in data.items():
        if hasattr(window, key):
            setattr(window, key, value)
    db.session.commit()
    return jsonify({'message': 'Window updated'})

@app.route('/api/windows/<int:id>', methods=['DELETE'])
def delete_window(id):
    window = Window.query.get_or_404(id)
    db.session.delete(window)
    db.session.commit()
    return jsonify({'message': 'Window deleted'})

@socketio.on('connect')
def handle_connect():
    print('Client connected')

@socketio.on('disconnect')
def handle_disconnect():
    print('Client disconnected')

if __name__ == '__main__':
    with app.app_context():
        db.create_all()
    socketio.run(app, debug=True)
