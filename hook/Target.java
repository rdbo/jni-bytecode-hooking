import java.io.File;

public class Target {
    public static native void myFunction(int number);

    public static native void myOtherFunction();
    

    public static void main(String[] args) {
        System.out.println("Target program started!");

        File libPath = new File("libtest.so");
        System.load(libPath.getAbsolutePath());

        myFunction(10); // this call should be hooked by the library when it happens!
    }
}
