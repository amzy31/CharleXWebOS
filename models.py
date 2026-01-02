from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()

class Window(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    window_id = db.Column(db.String(100), unique=True, nullable=False)
    left = db.Column(db.Integer, default=100)
    top = db.Column(db.Integer, default=100)
    width = db.Column(db.Integer, default=400)
    height = db.Column(db.Integer, default=300)
    z_index = db.Column(db.Integer, default=1000)
    maximized = db.Column(db.Boolean, default=False)
    minimized = db.Column(db.Boolean, default=False)
    prev_left = db.Column(db.Integer, nullable=True)
    prev_top = db.Column(db.Integer, nullable=True)
    prev_width = db.Column(db.Integer, nullable=True)
    prev_height = db.Column(db.Integer, nullable=True)
