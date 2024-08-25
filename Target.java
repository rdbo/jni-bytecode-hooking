import java.io.File;
import java.time.Duration;

public class Target {
    public static void myFunction(int number) {
        System.out.println("My function called!");
        System.out.println("My number is: " + number);
    }

    public static void myOtherFunction() {
        System.out.println("MY OTHER FUNC ORIGINAL");
    }

    public static void main(String[] args) throws Exception {
        System.out.println("Target program started!");

        File libPath = new File("libtest.so");
        System.load(libPath.getAbsolutePath());

        Thread.sleep(1000);

        myFunction(10); // this call should be hooked by the library when it happens!
        myOtherFunction();
    }
}
