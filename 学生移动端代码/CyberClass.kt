package com.example.cyberpodium_android

//上课
//实现：定位打卡考勤、编辑发送弹幕、判断对应课程是否在上课、课程下课后自动结束


//----------websocket------------

//location

import android.Manifest
import android.content.ContentValues.TAG
import android.content.Intent
import android.content.pm.PackageManager
import android.location.Criteria
import android.location.Location
import android.location.LocationManager
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.ListView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.example.cyberpodium_android.GetDistanceUtils.getDistanceMeter
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import org.json.JSONObject
import java.net.URISyntaxException
import java.util.*
import kotlin.concurrent.thread


class CyberClass :AppCompatActivity(){
    private var userID = "default";
    private var courseID = "default";
    private var coursename = "default";
    private var msginbox = " ";
    private var client: MyWebSocket? = null;

    //location
    private var lm: LocationManager? = null
    //my location
    private var myLongitude = 0.0   //WBS84经纬-经度
    private var myLatitude = 0.0 //WBS84经纬-纬度
    //东十九 location
    private val E19Longitude = 113.347692
    private val E19Latitude  = 23.140322
    //学院 location
    private val csLongitude = 113.344035
    private val csLatitude = 23.141302
    //一课 location
    private val centerLongitude =113.342509
    private val centerLatitude = 23.140657
    //private var listener: LocationListener? = null
    private var inArea = 0  //当前位置在三个定位之一的70米内
    //private lateinit var getLocation: GetLocation;

    private var exitAlready = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cyberclass);



        val titleTV: TextView = findViewById(R.id.course_name);
        val sendBtn: Button = findViewById(R.id.send_button);
        val msgET: EditText = findViewById(R.id.message_input);
        val backBtn: Button = findViewById(R.id.back_button);
//        val accountTV: TextView = findViewById(R.id.accountTextView);
//        val courselv: ListView = findViewById(R.id.courseListView);
//        val courseEditText : EditText = findViewById(R.id.courseInfoEditText);
//        val classBtn: Button = findViewById(R.id.attendButton);
//        val enterBtn: Button = findViewById(R.id.enterButton);
//        val delBtn: Button = findViewById(R.id.dropButton);
//        val addBtn: Button = findViewById(R.id.addCourseButton);

        // 获取传递过来的Intent
        val intent = getIntent()
        // 提取参数
        userID = intent.getStringExtra("userID")?:"nothing";
        courseID = intent.getStringExtra("courseID")?:"nothing";
        coursename = intent.getStringExtra("courseName")?:"nothing";

        //get phone's Location
        //startService(Intent(this, LocationService::class.java))
        val etAttResult : EditText = findViewById(R.id.ET_attendanceResult)
        lm = getSystemService(LOCATION_SERVICE) as LocationManager?
        //val provider: String? = lm?.getBestProvider(Criteria(), false)
        //listener = LocationListener()
        // 检查定位权限
        while(ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED
            && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            // 没有权限，进行权限请求
            val REQUEST_LOCATION_PERMISSION = 100
            ActivityCompat.requestPermissions(this, arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), REQUEST_LOCATION_PERMISSION)
            // 已经有权限，获取位置信息
//            val criteria = Criteria()
//            criteria.accuracy = Criteria.ACCURACY_COARSE
//            criteria.isAltitudeRequired = false //不要求海拔
//            criteria.isBearingRequired = false //不要求方位
//            criteria.isCostAllowed = true //允许有花费
//            criteria.powerRequirement = Criteria.POWER_LOW //低功耗
//            val provider = lm!!.getBestProvider(criteria, true)
//            Toast.makeText(this,"A定位的provider:$provider", Toast.LENGTH_SHORT).show()
//            val location: Location? = lm?.getLastKnownLocation(provider!!)
//            Toast.makeText(this,"location-$location", Toast.LENGTH_SHORT).show()
//            if (location != null) {
//                //不为空,显示地理位置经纬度
//                val longitude = "Longitude:" + location.getLongitude()
//                val latitude = "Latitude:" + location.getLatitude()
//                Toast.makeText(this,"getLastKnownLocation:$longitude-$latitude", Toast.LENGTH_SHORT).show()
//                //stopSelf()
//            }
            // 处理位置信息
        }
        //else {
            // 已经有权限，获取位置信息
            val criteria = Criteria()
            criteria.accuracy = Criteria.ACCURACY_FINE //使用高进度GPS
            criteria.isAltitudeRequired = false //不要求海拔
            criteria.isBearingRequired = false //不要求方位
            criteria.isCostAllowed = true //允许有花费
            criteria.powerRequirement = Criteria.POWER_LOW //低功耗
            val provider = lm!!.getBestProvider(criteria, true)
            Toast.makeText(this,"B定位的provider:$provider", Toast.LENGTH_SHORT).show()
            //val location: Location? = lm!!.getLastKnownLocation(provider!!)
            val location: Location? = lm!!.getLastKnownLocation(provider!!)
            Toast.makeText(this,"location-$location", Toast.LENGTH_SHORT).show()
            if (location != null) {
                //不为空,显示地理位置经纬度
                val longitude = "Longitude:" + location.getLongitude()
                val latitude = "Latitude:" + location.getLatitude()
                Toast.makeText(this,"getLastKnownLocation:$longitude-$latitude", Toast.LENGTH_SHORT).show()
                myLongitude = location.longitude.toDouble()
                myLatitude = location.latitude.toDouble()

                //myLongitude = 113.347108
                //myLatitude = 23.140531
                val dis1 = getDistanceMeter(myLongitude, myLatitude, E19Longitude, E19Latitude)
                Log.e(TAG, "东十九距离为：" + dis1 + "m");
                Toast.makeText(this,"东十九距离为:$dis1",Toast.LENGTH_SHORT).show()
                val dis2 = getDistanceMeter(myLongitude, myLatitude, csLongitude, csLatitude)
                Log.e(TAG, "学院距离为：" + dis2 + "m");
                val dis3 = getDistanceMeter(myLongitude, myLatitude, centerLongitude, centerLatitude)
                Log.e(TAG, "一课距离为：" + dis3 + "m");
                if(dis1<100||dis2<100||dis3<100)
                    inArea = 1;
                //stopSelf()
           }else Toast.makeText(this,"东十九距离为:????",Toast.LENGTH_SHORT).show()
        if(inArea==1){
            etAttResult.setText("定位打卡成功")
        }else{
            etAttResult.setText("定位打卡失败")
        }
        //location network module
        val jsonData = """  
            {  
                "userID":"$userID",
                "courseID": "$courseID",
                 "attResult":"$inArea"
            }  
            """
        val serverURL = "http://43.138.243.228/cyberpodium/And_interface/sendAttendanceResult.php";
        thread {
            try {
                val httpclient = OkHttpClient()
                //val requestBody=FormBody.Builder().add("a","1").add("b","2").build()
                val requestBody = jsonData.toRequestBody();
                val request = Request.Builder()
                    .url(serverURL)
                    .post(requestBody)
                    .build()
                val call=httpclient.newCall(request)
                val response = call.execute()//execute方法会阻塞在这里，必须等到服务器响应，得到response才会执行下面的代码
                val requestData = response.body?.string()
                val resultJSON = JSONObject(requestData);
                if (requestData != null) {
                    Log.e("postSync", requestData)
                    val mystatus = resultJSON["status"];
                    if(mystatus == "OK"){

                    }else{
                        //教师端不在线
                        val intent = Intent(this, MainActivity::class.java)
                        intent.putExtra("userID", userID);
                        startActivity(intent);
                        finish();

                    }
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }

        //websocket
        try {
            // 尝试建立WebSocket连接
            client = MyWebSocket("ws://43.138.243.228/ws?clientID=$userID&courseID=$courseID&isTeacher=0")
            client?.connect() // 安全调用connect方法，以防client为null
            //心跳包
            var time:Int = 0
            val timer = Timer()
            val task = object : TimerTask() {
                override fun run() {
                    if (client?.isOpen == true) {
                        client?.send("ping from user: $userID")
                    } else {
                        if(time>=1) {
                            if(!exitAlready){
                                exitAlready = true
                                runOnUiThread {
                                    Toast.makeText(this@CyberClass, "与服务器连接已断开", Toast.LENGTH_SHORT).show()
                                }
                                val intent = Intent(this@CyberClass, MainActivity::class.java)
                                intent.putExtra("userID", userID);
                                startActivity(intent);
                                finish()
                                timer.cancel()
                            }
                        }
                        time++
                    }
                }
            }
            timer.schedule(task, 0, 15 * 1000)
        } catch (e: URISyntaxException) {
            // 如果URL有语法问题，打印堆栈跟踪
            e.printStackTrace()
        }
        //websocket

        titleTV.setText(coursename);

        sendBtn.setOnClickListener {
            msginbox = msgET.text.toString();
            if(msginbox.length!=0){
                val jsonData = """  
                {"type": "sendToTeacher","clientID": "$userID","courseID": "$courseID","message": "$msginbox"}  
                """
                if(client!!.isOpen)
                    client?.send(jsonData);
                else{
                    Toast.makeText(this,"与服务器连接已断开",Toast.LENGTH_SHORT).show()
                    val intent = Intent(this@CyberClass, MainActivity::class.java)
                    intent.putExtra("userID", userID);
                    startActivity(intent);
                    finish()
                }
            }else{

            }

        }

        backBtn.setOnClickListener {
            exitAlready = true
            val intent = Intent(this, MainActivity::class.java)
            intent.putExtra("userID", userID);
            startActivity(intent);
            if (client?.isOpen == true) {
                client?.close();
            }
            finish();
        }



    }

    //Location






}

//采用Haversine算法(球型模型)而非Vincenty算法(椭球型算法)
//for caculate distance between 2 WBS84_location
//from https://blog.csdn.net/Life_s/article/details/106425523
object GetDistanceUtils {
    private val TAG = "GetDistanceUtils"
    private val EARTH_RADIUS = 6378.137 // 地球赤道半径
    private val PI = 3.14159265 //祖率

    /**
     * π是弧度制，180度是角度制，d*π／180,表示每角度等于多少弧度。
     */
    private fun rad(d: Double): Double {
        return d * PI / 180.0
    }

    //km
    fun getDistance(
        longitude1: Double, latitude1: Double,
        longitude2: Double, latitude2: Double
    ): Double {
        val Lat1 = rad(latitude1)
        val Lat2 = rad(latitude2)
        val a = Lat1 - Lat2
        val b = rad(longitude1) - rad(longitude2)
        var s = 2 * Math.asin(
            Math.sqrt(
                Math.pow(Math.sin(a / 2), 2.0)
                        + (Math.cos(Lat1) * Math.cos(Lat2)
                        * Math.pow(Math.sin(b / 2), 2.0))
            )
        )
        s = s * EARTH_RADIUS
        //有小数的情况;注意这里的10000d中的“d”
        s = Math.round(s * 10000.0) / 10000.0
        s = s * 1000 //单位：米
        //s = Math.round(s/0.1d) /10000d   ;//单位：千米 保留两位小数
        s = Math.round(s / 100.0) / 10.0 //单位：千米 保留一位小数
        return s
    }

    // meter
    fun getDistanceMeter(
        lng1: Double, lat1: Double,
        lng2: Double, lat2: Double
    ): Double {
        val radLat1 = rad(lat1)
        val radLat2 = rad(lat2)
        val a = radLat1 - radLat2
        val b = rad(lng1) - rad(lng2)
        var s = 2 * Math.asin(
            Math.sqrt(
                (Math.pow(Math.sin(a / 2), 2.0)
                        + (Math.cos(radLat1) * Math.cos(radLat2)
                        * Math.pow(Math.sin(b / 2), 2.0)))
            )
        )
        s = s * EARTH_RADIUS
        s = Math.round(s * 10000.0) / 10000.0
        s = s * 1000
        return s
    }

    /**
     *lat    给定的坐标纬度
     *lon    给定的坐标经度
     *radius 需要搜索的半径 比如 10Km
     *返回给定范围内 返回的最大小经度和最大小纬度
     */
    fun getAround(lat: Double, lon: Double, radius: Int): DoubleArray {
        val latitude = lat
        val longitude = lon
        val degree = (24901 * 1609) / 360.0
        val radiusMile = radius.toDouble()
        val dpmLat = 1 / degree
        val radiusLat = dpmLat * radiusMile
        val minLat = latitude - radiusLat
        val maxLat = latitude + radiusLat
        val mpdLng = degree * Math.cos(latitude * (PI / 180))
        val dpmLng = 1 / mpdLng
        val radiusLng = dpmLng * radiusMile
        val minLng = longitude - radiusLng
        val maxLng = longitude + radiusLng
        Log.e(TAG, "[$minLat,$minLng,$maxLat,$maxLng]")
        return doubleArrayOf(minLat, minLng, maxLat, maxLng)
    }
}


