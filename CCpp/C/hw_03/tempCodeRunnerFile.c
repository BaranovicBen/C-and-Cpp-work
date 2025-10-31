TDATE endDate(TDATE from, long long connections, unsigned perWorkDay, unsigned dowMask) {
    TDATE currentDate = from;

    while (connections > 0) {
        int dayOfWeek = getDayOfWeek(currentDate);

        // Check if the current day is a valid workday based on dowMask
        if ((dowMask & (1 << dayOfWeek)) == 0) {
            // Move to the next day without deduction if today is not in dowMask
            currentDate = incrementDate(currentDate);
            continue;
        }

        // Calculate daily frequency for the current workday
        int dailyFrequency = 0;
        if (dayOfWeek >= 1 && dayOfWeek <= 5) {  // Monday to Friday
            dailyFrequency = perWorkDay;         // Weekday
        } else if (dayOfWeek == 6) {             // Saturday
            dailyFrequency = (int)ceil(perWorkDay / 2.0);
        } else if (dayOfWeek == 0) {             // Sunday
            dailyFrequency = (int)ceil(perWorkDay / 3.0);
        }

        // Calculate next day's frequency without deducting it
        TDATE nextDate = incrementDate(currentDate);
        int nextDayOfWeek = getDayOfWeek(nextDate);
        int nextDayFreq = 0;
        if ((dowMask & (1 << nextDayOfWeek)) != 0) {  // Only proceed if next day is in dowMask
            if (nextDayOfWeek >= 1 && nextDayOfWeek <= 5) {
                nextDayFreq = perWorkDay;
            } else if (nextDayOfWeek == 6) {
                nextDayFreq = (int)ceil(perWorkDay / 2.0);
            } else if (nextDayOfWeek == 0) {
                nextDayFreq = (int)ceil(perWorkDay / 3.0);
            }
        }

        // Debug output
        printf("\nCurrent date: %d-%d-%d, dayOfWeek: %d, connections remaining: %lld\n", currentDate.m_Year, currentDate.m_Month, currentDate.m_Day, dayOfWeek, connections);
        printf("Daily frequency for today: %d, Next day frequency: %d\n", dailyFrequency, nextDayFreq);

        // Check if there are enough connections for the next day
        if (connections - nextDayFreq < 0) {
            printf("Not enough connections for next day. Last sufficient day reached: %d-%d-%d\n", currentDate.m_Year, currentDate.m_Month, currentDate.m_Day);
            return currentDate;
        }

        // Deduct connections for the current day
        connections -= dailyFrequency;
        printf("Connections remaining after deduction: %lld\n", connections);

        // If connections are exhausted after deduction, return the current date
        if (connections <= 0) {
            printf("Connections exhausted on date: %d-%d-%d\n", currentDate.m_Year, currentDate.m_Month, currentDate.m_Day);
            return currentDate;
        }

        // Move to the next day
        currentDate = nextDate;
    }

    printf("Date: %d-%d-%d\n", currentDate.m_Year, currentDate.m_Month, currentDate.m_Day);
    return currentDate;
}