class Account(object):
    interest = 0.02
    def __init__(self, account_holder):
        self.balance = 0
        self.holder = account_holder
    def deposit(self, amount):
        self.balance = self.balance + amount
        print("Yes!")

class CheckingAccount(Account):
    def __init__(self, account_holder):
        Account.__init__(self, account_holder)
    def deposit(self, amount):
        Account.deposit(self, amount)
        print("Have a nice day!")