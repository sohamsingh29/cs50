import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Getting user info
    user = db.execute("SELECT username,cash FROM users where id = :userid",userid=session["user_id"])

    # Getting stocks info
    stocks = db.execute("SELECT symbol,shares FROM stocks WHERE id = :userid",userid=session["user_id"])

    # To store all quotes    
    quote = {}

    # Assigning username
    username = user[0]["username"]

    # total cash
    total = 0

    # Totalling cash and storing Quotes
    for item in stocks :
        quote[item["symbol"]] = lookup(item["symbol"])
        total = total + (quote[item["symbol"]]["price"]*(int(item["shares"])))

    # getting remaining cash value 
    cash = user[0]["cash"]

    # Rendering template
    return render_template("index.html", cash=cash, quote=quote, username=username, stocks=stocks, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # checking if method is post
    if request.method == "POST":

        #storing Quote
        quote = lookup(request.form.get("symbol"))

        # checking for quote
        if quote == None :
            flash("invalid symbol")
            return redirect("/buy")

        # getting user info
        user = db.execute("SELECT * FROM users WHERE id = :userId",userId=session["user_id"])

        # getting cash value
        cash = int(user[0]["cash"])

        # Total prices of all the shares user want to buy
        tot_amount = (int(request.form.get("shares")))*quote["price"]

        # if cash is less than total price
        if tot_amount>cash:
            flash("not enough money")
            return redirect("/buy")

        # Evaluating cash
        cash = cash - tot_amount

        # Updating users cash value
        db.execute("UPDATE users SET cash = :cash where id = :userId",cash = cash,userId=session["user_id"])

        # Checking if stock exist
        if db.execute("SELECT * FROM stocks WHERE id = :userid and symbol = :symbol",userid=session["user_id"],symbol = request.form.get("symbol")):
            
            # getting stock info
            stock = db.execute("SELECT * FROM stocks WHERE id = :userid and symbol = :symbol",userid=session["user_id"],symbol = request.form.get("symbol"))

            # Updating stocks value
            stocks = int(stock[0]["shares"]) + int(request.form.get("shares"))

            db.execute("UPDATE stocks SET shares=:stocks WHERE id = :userId and symbol=:symbol",symbol=request.form.get("symbol"),userId=session["user_id"],stocks=stocks)

        # If stock doesn't exist
        else:
            
            # Inserting stock values
            db.execute("INSERT INTO stocks (id,symbol,shares) VALUES (:userId,:symbol,:stocks)",userId=session["user_id"],symbol = request.form.get("symbol"),stocks=request.form.get("shares"))
        # Inserting row in transactions table
        db.execute("INSERT INTO transactions (id,symbol,shares,purchase_sell_price) VALUES (:userId,:symbol,:shares,:price)",userId=session["user_id"],symbol = request.form.get("symbol"),shares=request.form.get("shares"),price=quote["price"])

        flash("bought it")

        return redirect("/")
    # if method is not post
    else :
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Getting transaction information from database
    transactions = db.execute("SELECT symbol,shares,purchase_sell_price,time_of_buying FROM transactions WHERE id = :userid",userid = session["user_id"])

    return render_template("history.html",transactions = transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # checking if method is post
    if request.method == "POST":
        
        # getting quote
        quote = lookup(request.form.get("symbol"))

        #Checking if quote exists
        if quote == None:
            return apology ("invalid symbol",533)

        return render_template("quoted.html",quote=quote)

    else :
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    session.clear()

    # checking if method is post
    if request.method == "POST":
        # checking for username and  password
        if not request.form.get("username"):
            return apology("must provide username",403)

        elif not request.form.get("password"):
            return apology("must provide password",403)

        elif not request.form.get("Cpassword")==request.form.get("password"):
            return apology("must confirm password",403)
        
        # generating hash password
        hashp = generate_password_hash(request.form.get("password"))
        
        # Creating a New user
        new_user = db.execute("INSERT INTO users (username,hash) VALUES(:name,:hashp)",name=request.form.get("username"),hashp=hashp)

        # if username's already taken
        if not new_user:
            return apology("username already taken",403)

        flash("Registered!")

        return redirect("/")

    # if method is not post
    else:

        return render_template("/register.html")




@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # checking if method is post
    if request.method == "POST":

        # Getting shares
        shares = int(request.form.get("shares"))

        # Getting symbol
        symbol = request.form.get("symbol")

        # Getting stock info from Database
        stock = db.execute("SELECT * FROM stocks WHERE id = :userId and symbol = :symbol",symbol=symbol,userId=session["user_id"])

        # Getting user info 
        user = db.execute("SELECT username,cash FROM users WHERE id = :userId",userId=session["user_id"])

        # Assigning Cash value
        cash = user[0]["cash"]

        # if no stock of that symbol 
        if stock == None :
            return apology("you don't own that stock",5)

        # if less shares owned
        if stock[0]["shares"]<shares:
            return apology("You don't have enough shares",6)

        # Getting Quote
        quote = lookup(symbol)

        # Total selling price
        sellPrice = shares*quote["price"]

        # Total user cash after selling stock
        cash_remaining = cash + sellPrice

        # Updating cash value of users
        user = db.execute("UPDATE users SET cash = :cash WHERE id = :userId",cash=cash_remaining,userId=session["user_id"])

        # if shares owned is same as shares selling
        if stock[0]["shares"] == shares :

            # Removing from stock table
            stock = db.execute("DELETE FROM stocks WHERE id = :userId and symbol = :symbol",userId=session["user_id"],symbol=symbol)

        else:
            
            # Calculating leftover shares 
            shares = -shares
            left = stock[0]["shares"] + shares

            # Updating stocks 
            stock = db.execute("UPDATE stocks SET shares=:shares WHERE id = :userId and symbol=:symbol", symbol=symbol,shares=left,userId=session["user_id"])

        # Inserting row in Transactions
        db.execute("INSERT INTO transactions (id,symbol,shares,purchase_sell_price) VALUES (:userId,:symbol,:shares,:price)",userId=session["user_id"],symbol=symbol,shares=shares,price=quote["price"])

        flash("SOLD!")

        return redirect("/")
    else:

        stocks = db.execute("SELECT * FROM stocks WHERE id = :userId", userId=session["user_id"])

        return render_template("sell.html",stocks=stocks)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
