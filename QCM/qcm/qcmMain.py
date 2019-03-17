import os
import logging

from flask import Flask, render_template, send_from_directory, request, session, url_for


class QCM:
    def __init__(self):
        self._qcm = self.create_app()
        self.add_routes(self._qcm)

    @property
    def qcm(self):
        return self._qcm

    @staticmethod
    def create_app(test_config=None):
        # create and configure the app
        app = Flask(__name__, instance_relative_config=True)
        app.config.from_mapping(
            SECRET_KEY='dev',
            DATABASE=os.path.join(app.instance_path, 'qcm.sqlite'),
            USERNAME='admin',
            PASSWORD='default')

        if test_config is None:
            # load the instance config, if it exists, when not testing
            app.config.from_pyfile('config.py', silent=True)
        else:
            # load the test config if passed in
            app.config.from_mapping(test_config)

        # ensure the instance folder exists
        try:
            os.makedirs(app.instance_path)
        except OSError:
            pass

        from . import db
        db.init_app(app)

        from . import auth
        app.register_blueprint(auth.bp)

        from . import blog
        app.register_blueprint(blog.bp)
        app.add_url_rule('/', endpoint='index')

        return app

    @staticmethod
    def add_routes(app):
        # a simple page that says hello
        @app.route('/hello')
        def hello():
            return 'Hello, World!'

        @app.route('/')
        def index():
            logging.debug('QCM: index')
            st = ""
            return render_template('status/index.html', status=st)

        @app.route('/jenkins')
        def jenkins():
            logging.debug('Jenkins: index')
            jks = ""
            return render_template('status/jenkins.html', check=jks)

        @app.route('/favicon.ico')
        def favicon():
            return send_from_directory(os.path.join(app.root_path, 'static'),
                                       'favicon.ico', mimetype='image/vnd.microsoft.icon')

        @app.route('/<logfile>.log')
        def lastlog(logfile):
            print("logfile: ", logfile)
            print("instance", os.path.join(app.instance_path, logfile))
            print("root path", app.root_path)
            return send_from_directory(app.instance_path, logfile+'.log',
                                       mimetype='text/plain')


