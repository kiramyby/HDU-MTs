#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <openssl/md5.h>

typedef int User;	// 0 Admin \ 1 Teacher \ 2 Student


typedef struct {
	char		name[10];
	int			length;
	StudentList*	nextStudent;	
}Course;

typedef struct {
	char			id[8];
	char			name[10];
	unsigned int	ave;
	CourseList* courseList;
}Student;

typedef struct {
	char			id[8];
	char			name[10];
	unsigned int	average;
	CourseList*		shallowCourseList;
}StudentList;


typedef struct {
	Course*	nextCourse;
}CourseList;

void Login();
void InputScore();

