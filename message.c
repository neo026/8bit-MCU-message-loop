#include "private.h"
#include "message.h"

#define MAX_MSG		6
static message_type msgQueque[MAX_MSG];	// message queque, this is a header of single link chain
static uint8 remainingMsg;				// the remaining message at the message queque.

void messageInit(void)
{
	remainingMsg = MAX_MSG;
	
	while(remainingMsg)
		msgQueque[-- remainingMsg].event = EventEmpty;
		
	// already clean the number of remaining message
}

/*
	brief: add a new message to the msgQueque arrary,
		if your input message is already put into the msgQueque, instead of it
		if you find an empty array position, that is the right position, fill it
			
	delay time = delay * 1ms
*/
void messageAdd(uint8 event, uint8 mdata, uint16 interval)
{
	uint8 i;

	// check input event is valid or not
	if(EventEmpty == event)
	{
		debugMsg("add invalid event:", event);
		return;
	}

	// check if this message already in the message queque
	for(i = 0; i < remainingMsg; i ++)
	{
		if(event == msgQueque[i].event)
		{
			msgQueque[i].msg = mdata;
			msgQueque[i].interval = interval;
			return ;
		}
	}

	// this message isn't in the message queque, check message length
	if((remainingMsg + 1) <= MAX_MSG)
	{
		msgQueque[remainingMsg].event = event;
		msgQueque[remainingMsg].msg = mdata;
		msgQueque[remainingMsg].interval = interval;
		// increase remaining message number
		remainingMsg ++;
	}
	else
	{
		//message queque is full
		runningStop(EER_MSG_FULL);
	}
}
/*
*	This will Clear all the same event in the message queque.
*/
void messageCancel(uint8 event)
{
	uint8 i;
	
	// check the message is valid or not
	if(EventEmpty == event)
	{
		debugMsg("cancel invalid event:", event);
		return;
	}
	
	// find the  cancel message
	for(i = 0; i < remainingMsg; i ++)
	{
		if(event == msgQueque[i].event)
		{
			// move the message queque
			for(; (i + 1) < remainingMsg; i ++)
				msgQueque[i] = msgQueque[i + 1];
			
			//clean the last message in the queque
			msgQueque[i].event = EventEmpty;

			//decrease the number of remaining message
			remainingMsg --;
			
			return ;
		}
	}

	//Run Here, this message you want to cancel is not the message queque. do nothing
}

/*
	Find the timeout message or the conndition = TRUE message, this function is put into the main() while(1).
	if find out, apply its handler, otherwise find next timeout or condition message.
*/
void messageLoop(void)
{
	uint8 i;
	uint8 needApply = FALSE;
	uint8 time = gProject.timer1ms;		// save time tick
	gProject.timer1ms = 0 ;				// restart time tick

	// decrease the interval time of message
	for(i = 0; i < remainingMsg; i ++)
	{
		if(time >= msgQueque[i].interval)
		{
			msgQueque[i].interval = 0;
			needApply = TRUE;
		}
		else
		{
			msgQueque[i].interval -= time;
		}
	}

	// check there is a message timeout. if not timeout, don't apply following checking for saving time
	if(FALSE == needApply)
		return;

checkAgain:
	for(i = 0; i < remainingMsg; i ++)
	{
		if(0 == msgQueque[i].interval)
		{
			message_type msgApply = msgQueque[i];
			
			// move the message queque
			for(; (i + 1) < remainingMsg; i ++)
				msgQueque[i] = msgQueque[i + 1];

			//clean the last message in the queque
			msgQueque[i].event = EventEmpty;
			remainingMsg --;
			
			// handle function
			messageHandler(&msgApply);

			/*the remainingMsg variable may be changed in handle function, 
			so we have to check message again from the fist one,
			this will be increase additional handling cycles
			*/
			goto checkAgain;
		}
	}	
}

