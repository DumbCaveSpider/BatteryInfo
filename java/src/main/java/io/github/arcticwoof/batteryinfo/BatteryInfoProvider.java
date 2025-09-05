package io.github.arcticwoof.batteryinfo;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.util.Log;

import org.geode.GeodeApp;

public class BatteryInfoProvider {
    private static final String TAG = "BatteryInfoProvider";

    public static int getBatteryLevel() {
        try {
            Context context = GeodeApp.get();
            IntentFilter iFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
            Intent batteryStatus = context.registerReceiver(null, iFilter);

            if (batteryStatus != null) {
                int level = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
                int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);

                if (level != -1 && scale != -1) {
                    return (int)((level / (float)scale) * 100f);
                }
            }
        } catch (Exception e) {
            Log.e(TAG, "Error getting battery level", e);
        }
        return -1;
    }

    public static boolean isCharging() {
        try {
            Context context = GeodeApp.get();
            IntentFilter iFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
            Intent batteryStatus = context.registerReceiver(null, iFilter);

            if (batteryStatus != null) {
                int status = batteryStatus.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
                return status == BatteryManager.BATTERY_STATUS_CHARGING ||
                       status == BatteryManager.BATTERY_STATUS_FULL;
            }
        } catch (Exception e) {
            Log.e(TAG, "Error checking charging status", e);
        }
        return false;
    }
}
