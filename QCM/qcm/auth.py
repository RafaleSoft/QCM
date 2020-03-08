import functools
import os
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText

from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for, current_app
)
from werkzeug.security import check_password_hash, generate_password_hash
from qcm.db import get_db
from smtplib import SMTP_SSL, SMTPException

bp = Blueprint('auth', __name__, url_prefix='/auth')


@bp.route('/register', methods=('GET', 'POST'))
def register():
    if request.method == 'POST':
        username = ''
        password = ''
        if request.form.__contains__('username'):
            username = request.form['username']
        if request.form.__contains__('password'):
            password = request.form['password']
        db = get_db()
        error = None

        if not username:
            error = 'Username is required.'
        elif not password:
            error = 'Password is required.'
        else:
            user = db.execute('SELECT id, password FROM users WHERE username = ?', (username,)).fetchone()
            if (user is not None) and (user['password'] == "reset"):
                db.execute('UPDATE users SET password = ? WHERE id = ?', (generate_password_hash(password), user['id'],))
                db.commit()
                flash("Mot de passe mise à jour.")
                return redirect(url_for('auth.login'))
            elif user is not None:
                error = 'L\'utilisateur {} est déjà enregistré.'.format(username)

        if error is None:
            firstname = ''
            lastname = ''
            email = ''
            sendevalemail = False
            sendrecallemail = False
            db.execute(
                'INSERT INTO users (username, password, firstname, lastname, email, sendevalemail, sendrecallemail) VALUES (?, ?, ?, ?, ?, ?, ?)',
                (
                    username, generate_password_hash(password), firstname, lastname, email, sendevalemail,
                    sendrecallemail))
            db.commit()
            return redirect(url_for('auth.login'))

        flash(error)

    return render_template('auth/register.html')


@bp.route('/login', methods=('GET', 'POST'))
def login():
    if request.method == 'POST':
        username = ''
        password = ''
        if request.form.__contains__('username'):
            username = request.form['username']
        if request.form.__contains__('password'):
            password = request.form['password']
        db = get_db()
        error = None
        user = db.execute('SELECT * FROM users WHERE username = ?', (username,)).fetchone()

        if user is None:
            error = 'Utilisateur incorrect.'
        elif len(password) < 1:
            error = 'Mot de passe vide.'
        elif not check_password_hash(user['password'], password):
            error = 'Mot de passe incorrect.'

        if error is None:
            session.clear()
            session['user_id'] = user['id']

            # ensure the user folder exists
            try:
                users_path = os.path.join(current_app.instance_path, 'users')
                os.makedirs(os.path.join(users_path, str(user['id'])))
            except OSError:
                pass

            return redirect(url_for('index'))

        flash(error)

    return render_template('auth/login.html')


@bp.route('/forget', methods=('GET', 'POST'))
def forget():
    if request.method == 'POST':
        username = ''
        if request.form.__contains__('username'):
            username = request.form['username']
        db = get_db()
        error = None
        user = db.execute('SELECT * FROM users WHERE username = ?', (username,)).fetchone()

        if user is None:
            error = 'Utilisateur ' + username + ' non enregistré.'
        elif len(username) < 1:
            error = 'Utilisateur invalide.'
        else:
            client = SMTP_SSL('smtp.orange.fr')

            text = MIMEText(
                '<html><body><h3>Cliquez sur le lien ci-dessous pour réinitialiser votre mot de passe Math QCM: </h3><p><a href=\"http://192.168.1.12:5000/auth/reinit?id=' +
                user['password'].split(':')[2] + '\">Reinitialiser</a></p></body></html>',
                'html',
                _charset='utf-8')
            message = MIMEMultipart()
            message['Subject'] = 'MathQCM Mot de passe oublié'
            message['From'] = 'fabfer@wanadoo.fr'
            message['To'] = user['email']
            message.attach(text)
            try:
                client.login('fabfer@wanadoo.fr', 'RaFaLe99A,NCK+"2=}')
                client.sendmail(message['From'], message['To'], message.as_string())
                client.quit()
                error = 'lien de réinitialisation de mot de passe envoyé à l\'adresse mail de: ' + username
            except SMTPException as e:
                error = 'Impossible d\'envoyer le mot de passe à l\'adresse mail de: ' + username

        session.clear()
        g.user = None
        flash(error)
        return redirect(url_for('auth.login'))

    return render_template('auth/login.html')


@bp.route('/reinit')
def reinit():
    id = ''
    if request.args.__contains__('id'):
        id = 'pbkdf2:sha256:'+request.args['id']
    db = get_db()

    error = None
    user = db.execute('SELECT * FROM users WHERE password = ?', (id,)).fetchone()
    if user is None:
        error = 'Utilisateur non enregistré.'
        flash(error)
        return redirect(url_for('auth.login'))
    elif id == user['password']:
        db.execute('UPDATE users SET password = \"reset\" WHERE id = ?', (user['id'],))
        db.commit()
        return redirect(url_for('auth.register'))
    else:
        return redirect(url_for('auth.login'))


@bp.before_app_request
def load_logged_in_user():
    user_id = session.get('user_id')

    if user_id is None:
        g.user = None
    else:
        g.user = get_db().execute('SELECT * FROM users WHERE id = ?', (user_id,)).fetchone()


@bp.route('/logout')
def logout():
    session.clear()
    g.user = None
    return redirect(url_for('auth.login'))


def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('auth.login'))

        return view(**kwargs)

    return wrapped_view
