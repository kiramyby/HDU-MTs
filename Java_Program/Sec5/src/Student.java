public class Student {
    protected String name, studentID, collegeName, birthDate;

    public Student(String name, String studentID, String collegeName, String birthDate) {
        this.name = name;
        this.studentID = studentID;
        this.collegeName = collegeName;
        this.birthDate = birthDate;
    }

    public void getInfo() {
        System.out.println(name + " " + studentID + " " + collegeName + " " + birthDate);
    }
}

class Undergraduate extends Student {
    public String counselor;

    public Undergraduate(String name, String studentID, String collegeName, String birthDate, String counselor) {
        super(name, studentID, collegeName, birthDate);
        this.counselor = counselor;
    }

    @Override
    public void getInfo() {
        System.out.println(name + " " + studentID + " " + collegeName + " " + birthDate + " " + counselor);
    }
}

class Graduate extends Student {
    public String supervisor;

    public Graduate(String name, String studentID, String collegeName, String birthDate, String supervisor) {
        super(name, studentID, collegeName, birthDate);
        this.supervisor = supervisor;
    }

    public void getInfo() {
        System.out.println(name + " " + studentID + " " + collegeName + " " + birthDate + " " + supervisor);
    }
}