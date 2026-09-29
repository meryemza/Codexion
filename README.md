*This project has been created as part of the 42 curriculum by `<Mezahir>`*

## 1. Description:

`Codexion` is a multithreaded programming project that simulates a group of coders competing for shared resources called dongles.

- Each coder must acquire two adjacent dongles before being able to compile. After compiling, the coder continues through debugging

and refactoring phases before attempting to compile again.

- The main goal of the project is to design a safe and efficient concurrent system where multiple coders can work in parallel

while correctly managing shared resources. The project focuses on thread synchronization, mutexes, condition variables,

resource management, and scheduling algorithms.

- Each dongle has its own waiting queue, and coders are scheduled using either FIFO or EDF (Earliest Deadline First) arbitration.

The implementation must prevent race conditions, deadlocks while ensuring that coders respect their burnout deadlines and dongle cooldown periods.

- A dedicated monitor thread continuously observes the state of all coders. It terminates the simulation when a coder burns out

or when all coders have successfully completed the required number of compilations. This ensures that the program stops correctly as soon as one

of the two possible end conditions is reached.

## 2. Instructions:

### A- Compilation

Clone the repository and navigate to the project directory:

```bash
git clone <repository-url>
```
Compile the project using the provided Makefile:

```bash
make
```
This generates the codexion executable.

To remove the compiled object files:

```bash
make clean
```
To remove all generated files, including the executable:

```bash
make fclean
```

```bash
To recompile the project: 
make re
```

   ### B- Execution
Run the program using the following syntax:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```bash
./codexion 2 200 20 20 20 2 10 edf
```
    
## 3. Resources:

    - https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/
    - https://www.geeksforgeeks.org/linux-unix/mutex-lock-for-linux-thread-synchronization/
    - https://www.youtube.com/watch?v=LjWug2tvSBU
    - https://medium.com/@akshatarhabib/understanding-threads-in-c-c9feb5e9372a
    - https://www.geeksforgeeks.org/c/multithreading-in-c/
    - https://www.geeksforgeeks.org/c/c-program-to-implement-priority-queue/
    - https://www.geeksforgeeks.org/c/c-program-to-implement-min-heap/
    - https://www.ibm.com/docs/fr/aix/7.3.0?topic=programming-using-mutexes
    - AI usage : ai was used at the beginning of the project to guide me on the concepts and topics i needed to study before starting the implementation.
      it was also used as a support to structure and improve the README documentation.

## 4. Blocking cases handled:

 **Deadlock prevention :** Coders take their two dongles in different orders depending on their ID. Even coders take right then left,
 while odd coders take left then right. This prevents coders from getting stuck waiting for each other.

**Cooldown handling :** When a coder releases a dongle, it cannot be immediately reused by another coder. The waiting coder remains blocked using pthread_cond_timedwait()
until the dongle's cooldown period has expired. Once the cooldown is over, the waiting thread can try to acquire the dongle.

**Shared data protection :** Shared data is protected using pthread_mutex_lock() and pthread_mutex_unlock(). This ensures that only one thread can access or modify shared
data at a time, preventing multiple coders from modifying the same data simultaneously and avoiding data races.

**Starvation prevention :** Coders waiting for a dongle are organized in a queue. `fifo` :the coder who requested the dongle first gets priority. With `edf`,
the coder with the closest deadline gets priority. This prevents a coder from waiting forever.

**Burnout and completion detection:** A monitor thread continuously checks whether a coder has burned out or whether all coders have completed the required number of compilations.
If a coder burns out, the monitor prints the burnout message and sets the stop_flag to stop the simulation.
If all coders complete their required compilations, the simulation also stops normally.

## 5. Thread synchronization mechanisms:

### threading primitives used:

* **`pthread_mutex_t`:** Used to protect shared data and prevent data races between threads.

  * **Dongle mutex:** Ensures that only one coder can use a dongle at a time.
  * **Logging mutex:** Prevents log messages from different threads from overlapping.
  * **Time mutex:** Protects shared time-related variables and coder state.
  * **Stop mutex:** Protects `stop_flag` when it is read or modified by the monitor and coder threads.

* **`pthread_cond_t`:** used to make threads wait for specific conditions without wasting CPU.

  * **Dongle condition:** When a coder cannot acquire a dongle, it waits using `pthread_cond_wait()` instead of continuously looping.
  * **Signaling:** When a dongle is released, `pthread_cond_broadcast()` wakes up waiting coders so they can check whether they can acquire it.

* **`pthread_cond_timedwait`:** Works just like a regular wait, but it stops waiting automatically after a specific time limit.

  * **Dongle cooldown:** When a coder needs to wait for the dongle cooldown, we don't block the CPU. The coder thread goes to sleep and wakes 
up automatically as soon as the cooldown time finishes.


### Race Condition Prevention

are prevented by protecting shared data with mutexes. For example, the coder1 and coder2 threads use the same mutex when accessing
last_compile_start and compiles_count ensuring that these values are not read while they are being modified

```c
pthread_mutex_lock(&(coder->data)->time);
coder->last_compile_start = get_current_time();
coder->compiles_count++;
pthread_mutex_unlock(&coder->data->time);
```
### Thread-Safe Communication Between Coders and the Monitor

The monitor communicates with coders through the shared stop_flag, which is protected by a mutex. When the monitor sets the flag
coders detect it and stop safely.

```c
pthread_mutex_lock(&data->stop);
if (data->stop_flag == 0)
data->stop_flag = 1;
pthread_mutex_unlock(&data->stop);
```