# rendezvous
class Barrier:
    def __init__(self, n):
        self.n = n
        self.count = 0
        self.mutex = Semaphore(1)
        self.turnstile = Semaphore(0)
        self.turnstile2 = Semaphore(0)
    
    def phase1(self):
        self.mutex.wait()
            self.count += 1
            if self.count == self.n 
                self.turnstile.signal(self.n)
        self.mutex.signal()
        self.turnstile.wait()
        self.turnstile.signal()
    
    def phase2(self):
        self.mutex.wait()
            self.count -= 1
            if self.count == 0
                self.turnstile2.signal(self.n)
        self.mutex.signal()
        self.turnstile2.wait()
        self.turnstile2.signal()
    
    def wait(self):
        self.phase1()
        self.phase2()

class Lightswitch:
    def __init__(self):
        self.counter = 0
        self.mutex = Semaphore(1)

    def lock(self, semaphore):
        self.mutex.wait()
            self.counter += 1
            if self.counter == 1
                semaphore.wait()
        self.mutex.signal()
    
    def unlock(self, semaphore):
        self.mutex.wait()
            self.counter -= 1
            if self.counter == 0
                semaphore.signal()
        self.mutex.signal()

