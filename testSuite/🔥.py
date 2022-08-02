nums = [i  for i in range(100)]
sum = 0 
for x in nums:
    if (x == 10):
        continue
    elif (x == 65):
        break
    sum = sum + x
print(sum)