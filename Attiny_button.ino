#define MOSFET_PIN 1
#define POWER_BUTTON_PIN 2
#define LED_PIN 4

bool _isMosfetEnabled = false;

#define BUTTON_PRESSED_MILLIS 2000
unsigned long _buttonPressStart;
bool _isButtonPressed = false;
int _currentState = 0;

void setup()
{
	pinMode(POWER_BUTTON_PIN, INPUT_PULLUP);
	pinMode(MOSFET_PIN, OUTPUT);
}

void loop()
{
	processButton(POWER_BUTTON_PIN);
}

void powerButtonPressed()
{
	_isMosfetEnabled = !_isMosfetEnabled;
	setPinValue(_isMosfetEnabled, MOSFET_PIN);
	setPinValue(_isMosfetEnabled, LED_PIN);
}

void setPinValue(bool value, int pin)
{
	if (value == true)
		digitalWrite(pin, HIGH);
	else
		digitalWrite(pin, LOW);
}

void processButton(int buttonPin)
{
	int buttonLevel = digitalRead(buttonPin);
	unsigned long currentTime = millis();

	//Check if button is pressed
	if (buttonLevel == LOW && _currentState == 0)
	{
		_buttonPressStart = currentTime;
		_currentState = 1;
	}
	//Wait for button to be pressed long enough to call function (>= BUTTON_PRESSED_MILLIS)
	else if (buttonLevel == LOW && _currentState == 1)
	{
		if (currentTime - _buttonPressStart >= BUTTON_PRESSED_MILLIS)
		{
			_currentState = 4;
			powerButtonPressed();
		}
	}
	//Check if button was released while press timer is active
	else if (buttonLevel == HIGH && _currentState == 1)
	{
		_currentState = 0;
	}
	//Wait for button to be released
	else if (buttonLevel == HIGH && _currentState == 4)
	{
		_currentState = 0;
	}	
}
