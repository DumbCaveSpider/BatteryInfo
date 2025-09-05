package io.github.arcticwoof.batteryinfo;

import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.os.PowerManager;
import android.provider.Settings;
import android.os.Build;

public class BatteryInfoProvider {
    
    /**
     * Get the current battery level as a percentage (0-100)
     * @return battery level percentage, or -1 if unknown
     */
    public static int getBatteryLevel() {
        Context context = /* get your application context */;
        IntentFilter iFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        Intent batteryStatus = context.registerReceiver(null, iFilter);
        
        if (batteryStatus == null) {
            return -1;
        }
        
        int level = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
        int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
        
        if (level == -1 || scale == -1) {
            return -1;
        }
        
        return (int)(((float)level / (float)scale) * 100.0f);
    }
    
    /**
     * Check if the device is currently charging
     * @return true if charging, false otherwise
     */
    public static boolean isCharging() {
        Context context = /* get your application context */;
        IntentFilter iFilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
        Intent batteryStatus = context.registerReceiver(null, iFilter);
        
        if (batteryStatus == null) {
            return false;
        }
        
        int status = batteryStatus.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
        return status == BatteryManager.BATTERY_STATUS_CHARGING ||
               status == BatteryManager.BATTERY_STATUS_FULL;
    }
    
    /**
     * Check if the device is in power saving mode (Battery Saver)
     * @return true if power saving mode is enabled, false otherwise
     */
    public static boolean isPowerSaveMode() {
        Context context = /* get your application context */;
        PowerManager powerManager = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        
        if (powerManager == null) {
            return false;
        }
        
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            return powerManager.isPowerSaveMode();
        }
        
        // For older Android versions that don't support power save mode
        return false;
    }
}
