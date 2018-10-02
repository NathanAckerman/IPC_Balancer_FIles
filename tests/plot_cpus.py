from collections import deque
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style

style.use('fivethirtyeight')

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)


def animate(i):
    graph_data = open('cpu_data.txt', 'r').read()
    lines = graph_data.split('\n')
    xs = deque([])
    y1s = deque([])
    y2s = deque([])
    y3s = deque([])
    y4s = deque([])
    count = 0
    for line in lines:
        if len(line)>1:
            y1, y2, y3, y4 = line.split(':')
            xs.append(count)
            count += 1
            y1s.append(int(y1))
            y2s.append(int(y2))
            y3s.append(int(y3))
            y4s.append(int(y4))
            if count > 40:
                xs.popleft()
                y1s.popleft()
                y2s.popleft()
                y3s.popleft()
                y4s.popleft()
    ax1.clear()
    ax1.plot(xs,y1s, label='cpu0')
    ax1.plot(xs,y2s, label='cpu1')
    ax1.plot(xs,y3s, label='cpu2')
    ax1.plot(xs,y4s, label='cpu3')
    ax1.legend(loc='upper left')

ani = animation.FuncAnimation(fig, animate, interval=250)
plt.show()











