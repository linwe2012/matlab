#include "pch.h"
#include "../matlab-core/matlab.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}
int my_argc;
char** my_argv;

TEST(Matrix, Matrix_IsGood) {
	V8Shell shell(my_argc, my_argv, std::cout);
	DefineJSMatrix(&shell);
	std::string str = "shell.runtest('matrix')";
	EXPECT_TRUE(shell.Execute(str));
}

int main(int argc, char** argv) {
	

	::testing::InitGoogleTest(&argc, argv);

	my_argc = argc;
	my_argv = argv;

	
	//int ret = 
	
	return RUN_ALL_TESTS();
}