interface Animal {
  void sound();
}

abstract class Dog implements Animal {
  public void sound() {
    System.out.println("Bark");
  }
}

public class Labrador extends Dog {
  public void labradorMethod() {
    System.out.println("Labrador");
  }
}

class Main {
  public static void main(String args[]) {
    Dog labrador = new Labrador();
    labrador.sound();
  }
}
