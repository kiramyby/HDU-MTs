class Human {
    protected String firstName, lastName, nationality;

    public Human(String firstName, String lastName, String nationality) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.nationality = nationality;
    }

    public void showNameInNativeLanguage() {
        System.out.println("Nationality: "+ nationality);
    }
}

class Chinese extends Human {
    public Chinese(String firstName, String lastName, String nationality) {
        super(firstName, lastName, nationality);
    }

    public void showNameInNativeLanguage(String prompt) {
        System.out.println(prompt + lastName + firstName);
    }
}

class American extends Human {
    public American(String firstName, String lastName, String nationality) {
        super(firstName, lastName, nationality);
    }

    public void showNameInNativeLanguage(String prompt) {
        System.out.println(prompt + firstName + " " + lastName);
    }
}