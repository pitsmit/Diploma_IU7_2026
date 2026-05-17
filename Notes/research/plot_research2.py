import matplotlib.pyplot as plt

xr = []
time_ms = []

with open("research2.csv") as f:
    next(f)
    for line in f:
        x, t = line.strip().split(",")
        xr.append(int(x))
        time_ms.append(float(t))

plt.plot(xr, time_ms, color="red")
plt.xlabel("Количество записей о монтировании, требующих обновления")
plt.ylabel("Время (мс)")
plt.grid(True)
plt.xlim(left=0, right=max(xr))
plt.ylim(bottom=min(time_ms), top=max(time_ms))

yticks = list(plt.yticks()[0])

if 350 in yticks:
    yticks.remove(350)
if 0 in yticks:
    yticks.remove(0)
if 50 in yticks:
    yticks.remove(50)

if max(time_ms) not in yticks:
    yticks.append(max(time_ms))
if min(time_ms) not in yticks:
    yticks.append(min(time_ms))

plt.yticks(sorted(yticks))

plt.show()

