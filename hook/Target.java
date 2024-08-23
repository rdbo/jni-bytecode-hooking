import java.io.File;

public class Target {
    public static native void myFunction();

    public static void main(String[] args) {
        System.out.println("Target program started!");

        var libPath = new File("libtest.so");
        System.load(libPath.getAbsolutePath());

        myFunction(); // this call should be hooked by the library when it happens!
    }
}
