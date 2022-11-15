x = -3
y = 1

a = min (abs(x), abs(y)) / max (abs(x), abs(y))
s = a * a
r = (((-0.0464964749 * s + 0.15931422) * s - 0.327622764) * s * a + a)

if abs(y) > abs(x):
     r = 1.57079637 - r

if x < 0:
    r = 3.14159274 - r

if y < 0:
    r = -r

print(r)

r = r - 1.57079637;
print(r)

if r > 0:
	r = 6.28318530 - r;

r = r * 8 / 3.14159274

print(r)