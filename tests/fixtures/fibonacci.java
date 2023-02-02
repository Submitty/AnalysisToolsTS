import java.io.*;

class Fibonacci {
    int count;

    public Fibonacci(int count) {
        this.count = count;
    }

    void printFibonacci() {
        int n1 = 0, n2 = 1, n3 = 0;
        if (count <= 0) {
            return;
        }
        System.out.print(n1 + " " + n2);
        for (int i = 2; i < count; ++i) {
            n3 = n1 + n2;
            System.out.print(" " + n3);
            n1 = n2;
            n2 = n3;
        }
    }
}

class Main {
    public static void main(String args[]) {
        Fibonacci fibonacci = new Fibonacci(10);
        fibonacci.printFibonacci();
    }
}
