import java.io.File;
import java.time.Duration;

public class Target {
    public static void myFunction() {
        System.out.println("My function called!");
    }

    public static void main(String[] args) throws Exception {
        System.out.println("Target program started!");

        File libPath = new File("libtest.so");
        System.load(libPath.getAbsolutePath());

        Thread.sleep(1000);

        myFunction(); // this call should be hooked by the library when it happens!
    }
}
