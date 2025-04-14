```
global buffer = queue()
**global lock = semaphore(1)**
**global items = semaphore(0)**
**local event**

def Producer:
    event = waitForEvent()
    **lock.wait()**
    buffer.add(event)
    **lock.signal()**
    **items.signal()**

def Consumer:
    **items.wait()**
    **lock.wait()**
    event = buffer.get()
    **lock.signal()**
    event.process()
```
