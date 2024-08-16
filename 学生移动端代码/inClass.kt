package com.example.cyberpodium_android

//进入课程
//实现：查看个人历史考勤记录

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.ArrayAdapter
import android.widget.Button
import android.widget.ListView
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import org.json.JSONObject
import kotlin.concurrent.thread

class inClass: AppCompatActivity() {
    private var userID = "default";
    private var courseID = "default";
    private var coursename = "default";
    private var recordshow  = ArrayList<String>();
    private lateinit var adapter: ArrayAdapter<String>;
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.inclass);

        //【初始化工作】
        val coursetitle:TextView = findViewById(R.id.label_zero)
        val attendanceLV : ListView = findViewById(R.id.attendance_list)
        val backBTN : Button = findViewById(R.id.back_button)
        //从外部获取
        // 获取传递过来的Intent
        val intent = getIntent()
        // 提取参数
        userID = intent.getStringExtra("userID")?:"nothing";
        courseID = intent.getStringExtra("courseID")?:"nothing";
        coursename = intent.getStringExtra("courseName")?:"nothing";

        coursetitle.setText(coursename);
        //【获取考勤信息】
        adapter= ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,recordshow);
        attendanceLV.adapter = adapter;
        getMSG()
        //【返回按钮监听】
        backBTN.setOnClickListener {
            val intent = Intent(this, MainActivity::class.java)
            intent.putExtra("userID", userID);
            startActivity(intent);
            finish();
        }
    }
    private fun getMSG(){
        val jsonData = """  
            {  
                "userID":"$userID",
                "courseID": "$courseID"
            }  
            """
        val serverURL = "http://43.138.243.228/cyberpodium/And_interface/getMyAttendance.php";
        thread {
            try {
                val httpclient = OkHttpClient()
                val requestBody = jsonData.toRequestBody();
                val request = Request.Builder()
                    .url(serverURL)
                    .post(requestBody)
                    .build()
                val call=httpclient.newCall(request)
                val response = call.execute()//execute方法会阻塞在这里，必须等到服务器响应，得到response才会执行下面的代码
                val requestData = response.body?.string()
                val resultJSON = JSONObject(requestData)
                if (requestData != null) {
                    Log.e("postSync", requestData)
                    val mystatus = resultJSON["status"];
                    if(mystatus == "success"){
                        recordshow.clear()
                        var item = resultJSON["record"].toString()
                        val array = item.split(",").toTypedArray()
                        var i:Int = 0;
                        for(a in array){
                            i++
                            var myitem : String = "第"
                            myitem+=i.toString()
                            myitem+="次: \t"
                            if(a=="0")
                                myitem+="缺勤"
                            else myitem+="已签到"
                            recordshow.add(myitem)
                        }
                    }

                }

            } catch (e: Exception) {
                e.printStackTrace()
            }
            runOnUiThread {
                // 在这里更新你的adapter和UI
                adapter.notifyDataSetChanged();

            }
        }

    }



}