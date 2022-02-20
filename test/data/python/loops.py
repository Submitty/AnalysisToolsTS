# python3

print("test with while and for loops")

i = 0
while i < 5:
  print(i)
  i += 1

for i in range(5):
  print(f"i = {i}")

for i in range(5):
  j = 0
  while j < 0:
    print(i, j)
    for k in range(5):
      print(f"i = {i}, j = {j}, k = {k}")
    j += 1
