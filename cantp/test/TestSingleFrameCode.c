//#include "ProductionCode.h"
#include "unity.h"
#include "unity_fixture.h"
#include "test_code_interface.h"


TEST_GROUP(TpSingleFrame);
TEST_GROUP(TpMultiFrame);


void cantp_run_times(U32 cnt)
{
	U32 i;
	for (i = 0; i < cnt; i++)
	{
		cantp_main_function();
	}
}


//sometimes you may want to get at local data in a module.
//for example: If you plan to pass by reference, this could be useful
//however, it should often be avoided
extern int Counter;

TEST_SETUP(TpSingleFrame)
{
  //This is run before EACH TEST
  //Counter = 0x5a5a;
	can_driver_init_disable_tx_confirm();
	cantp_init();
}

TEST_TEAR_DOWN(TpSingleFrame)
{
}

TEST(TpSingleFrame, TestSFReceptionFlow)
{

	can_driver_sf_rx_isr_2();
}

TEST(TpSingleFrame, TestSFDataLength)
{
  //All of these should pass
  TEST_ASSERT_EQUAL(1, check_sf_dl(10,13,0));
  //TEST_ASSERT_EQUAL(0, FindFunction_WhichIsBroken(1));
 // TEST_ASSERT_EQUAL(0, FindFunction_WhichIsBroken(33));
 // TEST_ASSERT_EQUAL(0, FindFunction_WhichIsBroken(999));
 // TEST_ASSERT_EQUAL(0, FindFunction_WhichIsBroken(-1));
}


TEST(TpSingleFrame, TestSFTransmission)
{
	U32 i = 202;

	dcm_tmp_transmit_response(6);

	while (i--)
	{
		cantp_main_function();
	}
	

}


TEST(TpSingleFrame, TestFFTransmission)
{
	dcm_tmp_transmit_response(20);

	cantp_main_function();
}

TEST_SETUP(TpMultiFrame)
{
	//This is run before EACH TEST
	//Counter = 0x5a5a;
	can_driver_init_normal();
	cantp_init();
}

TEST_TEAR_DOWN(TpMultiFrame)
{
}

TEST(TpMultiFrame, TestMultiFrameTransmission)
{
	dcm_tmp_transmit_response(255);
	cantp_run_times(1);

	cantp_run_times(1);
	can_fc_rx_1();
	cantp_run_times(5);

	can_fc_rx_1();
	cantp_run_times(5);

	can_fc_rx_1();
	cantp_run_times(5);

	can_fc_rx_1();
	cantp_run_times(5);

	can_fc_rx_1();
	cantp_run_times(5);

	can_fc_rx_1();
	cantp_run_times(5);
	cantp_run_times(12);
}
TEST(TpMultiFrame, TestMultiFrameReception)
{
	U32 i = 202;
	//1.请求多帧发送,data size = 20
	dcm_tmp_transmit_response(20);
	//2.发送第一帧 FF
	cantp_main_function();
	//3.等待接收者发送流控帧,接收者超时未发送FC帧
	while (i--)
	{
		cantp_main_function();
	}


}



#if 0

TEST(ProductionCode, FindFunction_WhichIsBroken_ShouldReturnTheIndexForItemsInList_WhichWillFailBecauseOurFunctionUnderTestIsBroken)
{
  // You should see this line fail in your test summary
  TEST_ASSERT_EQUAL(1, FindFunction_WhichIsBroken(34));

  // Notice the rest of these didn't get a chance to run because the line above failed.
  // Unit tests abort each test function on the first sign of trouble.
  // Then NEXT test function runs as normal.
  TEST_ASSERT_EQUAL(8, FindFunction_WhichIsBroken(8888));
}

TEST(ProductionCode, FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValue)
{
    //This should be true because setUp set this up for us before this test
    TEST_ASSERT_EQUAL_HEX(0x5a5a, FunctionWhichReturnsLocalVariable());

    //This should be true because we can still change our answer
    Counter = 0x1234;
    TEST_ASSERT_EQUAL_HEX(0x1234, FunctionWhichReturnsLocalVariable());
}

TEST(ProductionCode, FunctionWhichReturnsLocalVariable_ShouldReturnTheCurrentCounterValueAgain)
{
    //This should be true again because setup was rerun before this test (and after we changed it to 0x1234)
    TEST_ASSERT_EQUAL_HEX(0x5a5a, FunctionWhichReturnsLocalVariable());
}

TEST(ProductionCode, FunctionWhichReturnsLocalVariable_ShouldReturnCurrentCounter_ButFailsBecauseThisTestIsActuallyFlawed)
{
    //Sometimes you get the test wrong.  When that happens, you get a failure too... and a quick look should tell
    // you what actually happened...which in this case was a failure to setup the initial condition.
    TEST_ASSERT_EQUAL_HEX(0x1234, FunctionWhichReturnsLocalVariable());
}

#endif
