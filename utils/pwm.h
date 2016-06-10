
#ifndef _PWM_H
#define _PWM_H


#define PWM_STEP	20			// 10ms as a PWM period
#define PWM_PERIOD	PWM_STEP	// the same as PWM steps

#define FADE_TIME(x)	(uint8)((x)/PWM_STEP)

typedef struct
{
	uint8 led;			// select a led to display
	uint8 period;		// period time of a cycle
	uint8 ratio;		// ratio of a cycle
	uint8 time;			// a temp variable for counting the time (unit 1ms)
	uint8 onOff;		// turn on or off the selected led.
}pwm_type;

typedef struct
{
	uint8 led;			// select a led to fade on or fade off
	uint8 direction;	// direction of fade, direction equals ON, that will fade on, direction is equals OFF, that will fade off
	uint8 step;			// current step;
	uint16 repeat;		// the repeat time (or keep time) of very step, unit 1ms
	//pwm
	uint8 pwmRatio;		
	uint16 pwmRepeat;
	uint8 pwmTime;
	uint8 onOff;
}fade_type;

void ledPwmStart(uint8 led, uint8 period, uint8 ratio);
void ledPwmStop(void);
void ledPwmLoop(void);

void ledFadeStart(uint8 led, uint16 fade_time);
void ledFadeStop(void);
void ledFadeLoop(void);

#endif // _PWM_H

