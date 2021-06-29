#include <iostream>
#include <time.h>
#include <conio.h>
using namespace std;

#define Q_LIMIT 100
#define BUSY 1
#define IDLE 0
float D1, D2;
int Count = 0, ch = 1;
class Server
{
    int next_event_type, num_cust_delayed, num_delays_required, num_events, num_in_q_1, num_in_q_2, server_status_1, server_status_2;
    float area_num_in_q_1, area_num_in_q_2, area_server_status_1, area_server_status_2, mean_interarrival, mean_service, sim_time;
    float time_arrival[Q_LIMIT + 1], time_last_event, time_next_event[3], total_delay, delay_S1, delay_S2;
public:
    void main_program();
    void initialize();
    void timing();
    void arrival();
    void departure();
    void report();
    void event();
    float library(float mean);
    float expon(float mean_1);
    int choose();
    float fuel_consumption(int N);
};
void Server::main_program()
{
    num_events = 2;
    cout << "\n\t\t*** Multi-server queueing system ***\n";
    cout << "  Please provide the given details: " << endl;
    getch();
    cout << "\n  Mean inter-arrival time= ";
    cin >> mean_interarrival;
    cout << "\n  Mean service time= ";
    cin >> mean_service;
    cout << "\n  Number of customers= ";
    cin >> num_delays_required;
    initialize();
    while (num_cust_delayed < num_delays_required)
    {
        timing();
        event();
        switch (next_event_type)
        {
        case 1:arrival();
            break;
        case 2:departure();
            break;
        }
    }
    report();
    cout << "\n  Press any key to continue!! ";
    getch();
}
void Server::initialize()
{
    sim_time = 0.0;

    server_status_1 = IDLE;
    server_status_2 = IDLE;
    num_in_q_1 = 0;
    num_in_q_2 = 0;
    time_last_event = 0.0;

    num_cust_delayed = 0;
    total_delay = 0.0;
    delay_S1 = 0.0;
    delay_S2 = 0.0;
    area_num_in_q_1 = 0.0;
    area_num_in_q_2 = 0.0;
    area_server_status_1 = 0.0;
    area_server_status_2 = 0.0;

    time_next_event[1] = sim_time + library(mean_interarrival);
    time_next_event[2] = 1.0e+40;
}
void Server::timing()
{
    int i;
    float min_time_next_event = 1.0e+39;
    next_event_type = 0;

    for (i = 1; i <= num_events; ++i)
    {
        if (time_next_event[i] < min_time_next_event)
        {
            min_time_next_event = time_next_event[i];
            next_event_type = i;
        }
    }
    if (next_event_type == 0)
    {
        cout << "\n  Event list empty at time " << sim_time;
        exit(1);
    }
    sim_time = min_time_next_event;
}
void Server::event()
{
    float time_lag;
    time_lag = sim_time - time_last_event;
    time_last_event = sim_time;

    area_num_in_q_1 += num_in_q_1 * time_lag;
    area_num_in_q_2 += num_in_q_2 * time_lag;

    area_server_status_1 += server_status_1 * time_lag;
    area_server_status_2 += server_status_2 * time_lag;
}
void Server::arrival()
{
    time_next_event[1] = sim_time + library(mean_interarrival);
    if (server_status_1 == BUSY && server_status_2 == BUSY)
    {
        switch (ch)
        {

        case 1: ++num_in_q_1;
            if (num_in_q_1 > Q_LIMIT)
            {
                cout << "\n  Overflow!! of the array time-arrival at " << sim_time;
                exit(2);
            }
            time_arrival[num_in_q_1] = sim_time;
            ch = 2;
            break;
        case 2: ++num_in_q_2;
            if (num_in_q_2 > Q_LIMIT)
            {
                cout << "\n  Overflow!! of the array time-arrival at " << sim_time;
                exit(2);
            }
            time_arrival[num_in_q_2] = sim_time;
            ch = 1;
            break;
        default: break;
        }
    }
    else if (server_status_1 == IDLE && server_status_2 == BUSY)
    {
        ++num_cust_delayed;
        server_status_1 = BUSY;

        time_next_event[2] = sim_time + library(mean_service);
        D1 = time_next_event[2];
    }
    else if (server_status_2 == IDLE && server_status_1 == BUSY)
    {
        ++num_cust_delayed;
        server_status_2 = BUSY;

        time_next_event[2] = sim_time + library(mean_service);
        D2 = time_next_event[2];
    }
    else
    {
        ++num_cust_delayed;
        server_status_1 = BUSY;

        time_next_event[2] = sim_time + library(mean_service);
        D1 = time_next_event[2];
    }
}
void Server::departure()
{
    int i;
    float delay;

    if (num_in_q_1 == 0 && num_in_q_2 == 0)
    {
        server_status_1 = IDLE;
        server_status_2 = IDLE;
        time_next_event[2] = 1.0e+40;
    }
    if (num_in_q_1 > 0)
    {
        --num_in_q_1;
        if (D1 == time_next_event[2])
        {
            delay = sim_time - time_arrival[1];
            delay_S1 += delay;
            ++num_cust_delayed;
            time_next_event[2] = sim_time + library(mean_service);
            for (i = 1; i <= num_in_q_1; ++i)
                time_arrival[i] = time_arrival[i + 1];
        }
    }
    else if (num_in_q_2 > 0)
    {
        --num_in_q_2;
        if (D2 == time_next_event[2])
        {
            delay = sim_time - time_arrival[1];
            delay_S2 += delay;

            ++num_cust_delayed;
            time_next_event[2] = sim_time + library(mean_service);
            for (i = 1; i <= num_in_q_1; ++i)
                time_arrival[i] = time_arrival[i + 1];
        }
    }
}
void Server::report()
{
    cout << "\n  Simulation time= " << sim_time;
    total_delay = area_num_in_q_1 + area_num_in_q_2;
    cout << "\n  total delay= " << total_delay;
    cout << "\n  Average delay in queue= " << total_delay / num_cust_delayed;
    cout << "\n  Waiting time of customers in Queue 1= " << area_num_in_q_1;
    cout << "\n  Waiting time of customers in Queue 2= " << area_num_in_q_2;
    cout << "\n  Server 1 utilization= " << area_server_status_1 / sim_time;
    cout << "\n  Server 2 utilization= " << area_server_status_2 / sim_time;
    float Fuel;
    Fuel = fuel_consumption(num_delays_required);
    cout << "\n  Total fuel consumption= " << Fuel;
}
float Server::library(float mean)
{
    float M;
    M = expon(mean);
    Count++;
    return (M);

}
float Server::expon(float mean_1)
{
    srand((unsigned)time(NULL));
    float m, Array[100];
    for (int i = 0; i <= 100; i++)
    {
        m = ((float)rand() / RAND_MAX);
        m *= mean_1;
        Array[i] = m;
    }
    return Array[Count];
}
float Server::fuel_consumption(int N)
{
    srand((unsigned)time(NULL));
    int Array[N], m;
    int sum = 0;
    cout << endl;
    for (int i = 0; i < N; i++)
    {
        m = rand() / 1000;
        if (m == 0)
            Array[i] = m + 10;
        else
            Array[i] = m;
        sum += Array[i];
    }
    return sum;
}
int main()
{
    Server s;
    s.main_program();
    return 0;
}

