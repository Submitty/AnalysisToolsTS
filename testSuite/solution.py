class Animal:
    age = 10
    def age(self, age):
        self.age = age
    def dog(self):
        print("woof")

dog = Animal()
age = 1
dog.age(age)
dog.dog()
print(dog.age)
