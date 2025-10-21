#include <iostream>
#include<conio.h>
#include<thread>
#include<chrono>
using std::cin;
using std::cout;
using std::endl;

#define Enter 13
#define Escape 27



#define MIN_TANK_VOLUM 25
#define MAX_TANK_VOLUM 125
class Tank
{
	const int VOLUME;
	double fuel_level;
public:
	int get_VOLUME()const
	{
		return VOLUME;
	}
	double get_fuel_level()const
	{
		return fuel_level;
	}
	void fill(double fuel)
	{
		if (fuel < 0)return;
		fuel_level += fuel;
		if (fuel_level > VOLUME)fuel_level = VOLUME;
	}
	void consuption(double volume)
	{
		if (volume >= fuel_level || volume >= VOLUME)
		{
			fuel_level = 0;
			return;
		}
		fuel_level -= volume;
	}
	Tank(int volume) :
		VOLUME
		(
			volume < MIN_TANK_VOLUM ? MIN_TANK_VOLUM :
			volume > MAX_TANK_VOLUM ? MAX_TANK_VOLUM :
			volume
		)
	{
		//this->VOLUME = volume;
		this->fuel_level = 0;
		cout << "Tank is ready:\t" << this << endl;
	}
	~Tank()
	{
		cout << "Tank is over:\t" << this << endl;
	}
	void info()const
	{
		cout << "volume:\t" << VOLUME << " liters\n";
		cout << "fuel:\t" << get_fuel_level() << " liters\n";
	}
};

#define MIN_ENGINE_CONSUPTION 5
#define MAX_ENGINE_CONSUPTION 25
#define ENGINE_CONSUPTION_IDLING 0.0003
class Engine
{
	double consuption_per_second;
	bool is_started;
public:
	const double CONSUMPTION;
	double get_consuption_per_second()const
	{
		return consuption_per_second;
	}
	Engine(double consuption) :
		CONSUMPTION
		(
			consuption<MIN_ENGINE_CONSUPTION ? MIN_ENGINE_CONSUPTION :
			consuption>MAX_ENGINE_CONSUPTION ? MAX_ENGINE_CONSUPTION :
			consuption
		)
	{
		consuption_per_second = CONSUMPTION * 3e-5;
		is_started = false;
		cout << "Engine is ready:\t" << this << endl;
	}
	~Engine()
	{
		cout << "Engine is over:\t" << this << endl;
	}

	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()const
	{
		return is_started;
	}

	void info()const
	{
		cout << "Consuption per 100 km:\t" << CONSUMPTION << " liters.\n";
		cout << "Consuption per 1 sec:\t" << get_consuption_per_second() << " liters.\n";
	}
};

#define MAX_SPEED_LOWER_LIMIT 40
#define MAX_SPEED_UPPER_LIMIT 400
class Car
{
	Engine engine;
	Tank tank;
	const int MAX_SPEED;
	int speed;
	bool driver_inside;
	bool in_motion;
public:
	Car(double consuption, int volume, int max_speed) :
		engine(consuption),
		tank(volume),
		speed(0),
		MAX_SPEED
		(
			max_speed<MAX_SPEED_LOWER_LIMIT ? MAX_SPEED_LOWER_LIMIT :
			max_speed>MAX_SPEED_UPPER_LIMIT ? MAX_SPEED_UPPER_LIMIT :
			max_speed
		)
	{
		driver_inside = false;
		in_motion = false;
		cout << "Your car is ready:-)\t" << this << endl;
	}
	~Car()
	{
		cout << "Your car is over:\t" << this << endl;
	}

	void get_in()
	{
		driver_inside = true;
	}
	void get_out()
	{
		driver_inside = false;
	}
	void consuptin()
	{
		if (engine.started())
		{
			if (in_motion)tank.consuption(engine.get_consuption_per_second());
			else tank.consuption(ENGINE_CONSUPTION_IDLING);
		}

		if (tank.get_fuel_level() == 0)
		{
			engine.stop();
			in_motion = false;
		}
	}


	void control()
	{

		char key;
		do
		{
			if (engine.started())
				consuptin();



			panel();
			cout << "Press 'Enter' to get in\nPress 'Q' to start engine\nPress 'W' to start motion\nPress 'S' to stop motion\nPress 'F' to fill up the tank" << endl;
			key = _getch();
			switch (key)
			{
			case Enter:
				if (!in_motion)
				{
					if (driver_inside)get_out();
					else get_in();
				}
				break;
			case 'Q':
			case 'q':
				if (driver_inside && tank.get_fuel_level() > 0)
				{
					if (!engine.started())
					{
						engine.start();
						cout << "Engine start\n";
					}
					else
					{
						engine.stop();
						cout << "Engine stoped\n";
					}
				}
				break;
			case 'W':
			case 'w':
				if (driver_inside && engine.started())
					in_motion = true;
				break;
			case 'S':
			case 's':
				if (driver_inside)
					in_motion = false;
				break;
			case 'F':
			case 'f':
				if (!in_motion)
				{
					double amount;
					cout << "How much do you want? "; cin >> amount;
					tank.fill(amount);
				}
				break;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
		} while (key != Escape);
	}
	void panel()const
	{
		cout << "=================================================================\n";
		cout << "Fuel level:\t" << tank.get_fuel_level() << " liters.\n";
		cout << "Engine is:\t" << (engine.started() ? "started" : "stoped") << endl;
		cout << "Driver inside is:\t" << (driver_inside ? "yes" : "no") << endl;
		cout << "=================================================================\n";
	}
	void info()const
	{
		engine.info();
		tank.info();
		cout << "Max speed:\t" << MAX_SPEED << " km/h\n";
	}

};


//#define TANK_CHECK
//#define ENGINE_CHECK

int main()
{
	setlocale(LC_ALL, "");

#ifdef TANK_CHECK
	Tank tank(-30);
	tank.info();
	double fuel;
	do
	{
		cout << "¬ведите обьем топлива: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (fuel > 0);
#endif

#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif 

	Car bmw(15, 80, 270);
	bmw.info();
	bmw.control();

}

