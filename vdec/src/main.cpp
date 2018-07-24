#include "ScreenSaverPlayer.h"
#include <signal.h>

static int gQuit = 0;
void my_handler(int param)
{
	if (gQuit == 0)
	{
		gQuit = 1;
	}
	else
	{
		exit(1);
	}
}

int main(int argc,char *argv[])
{
	signal(SIGINT, my_handler);
	ScreenPlayer::GetInstance()->OpenDynimicScreen("/tmp/ds.h264");
	while(!gQuit)
		sleep(1);
	ScreenPlayer::GetInstance()->CloseDynimicScreen();
	return 0;
}

