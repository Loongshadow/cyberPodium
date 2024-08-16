package com.example.cyberpodium_android

//cyberpodium android 主界面-用户信息&课程信息
//主界面
//实现：显示用户信息、课程信息，支持选课、退课、进入课程、开始上课、修改用户信息(昵称)

import android.app.AlertDialog
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.*
import com.google.gson.Gson
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import org.json.JSONObject
import org.w3c.dom.Text
import kotlin.concurrent.thread
import kotlin.system.exitProcess
import com.google.gson.annotations.SerializedName
import com.google.gson.JsonObject
import com.google.gson.JsonParser
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

//for save course item for course list
data class courseJsonItem(
    var courseID:String,
    var courseName:String,
    var time:String,
    var place:String,
    var builder:String,
    var remark:String
)


class MainActivity : AppCompatActivity(){
    var userID = "default";
    //lateinit var userName : String;
    var  courseList = ArrayList<courseJsonItem>();
    var courseshow  = ArrayList<String>();
    private lateinit var adapter: ArrayAdapter<String>;
    //val adapter= ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,courseshow);
    var userName = "default";
    var currentCourseID = " ";
    var currentCourseName = "";

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // 获取传递过来的Intent
        val intent = getIntent()
        // 提取参数
        userID = intent.getStringExtra("userID")?:"nothing";

        val exitBtn:Button = findViewById(R.id.exitButton);
        val editBtn:Button = findViewById(R.id.editButton);
        val nameET:EditText = findViewById(R.id.nicknameEditText);
        val accountTV:TextView = findViewById(R.id.accountTextView);
        val courselv:ListView = findViewById(R.id.courseListView);
        val courseEditText : EditText = findViewById(R.id.courseInfoEditText);
        val classBtn:Button = findViewById(R.id.attendButton);
        val enterBtn:Button = findViewById(R.id.enterButton);
        val delBtn:Button = findViewById(R.id.dropButton);
        val addBtn:Button = findViewById(R.id.addCourseButton);


        accountTV.setText(userID);
        println(userID);


        adapter= ArrayAdapter<String>(this,android.R.layout.simple_list_item_1,courseshow);
        courselv.adapter = adapter;
        //getMSG();
        getMSG();
        Log.e("bye","bye");

        //click item in listview
        courselv.setOnItemClickListener { parent, view, position, id ->
            // 在这里处理点击事件
            // position 参数表示被点击项的索引
            Toast.makeText(this, "你点击了位置为 $position 的项", Toast.LENGTH_SHORT).show()
            val coursemsg = courseshow[position];
            courseEditText.setText(coursemsg);
            currentCourseID = courseList[position].courseID;
            currentCourseName = courseList[position].courseName;

        }

        //finish app
        exitBtn.setOnClickListener {
            finish();
        }
        //edit nicknames
        editBtn.setOnClickListener {
            val btntext = editBtn.text.toString()
            val oldname = nameET.text.toString()
            if(btntext == "编辑"){
                editBtn.setText("保存")
                nameET.isEnabled = true
            }else if(btntext == "保存"){
                editBtn.setText("编辑")
                nameET.isEnabled = false
                val newname: String = nameET.text.toString()
                val jsonData = """  
                {  
                    "ID":"$userID",
                    "Name": "$newname"  
                }  
                """
                val serverURL = "http://43.138.243.228/cyberpodium/And_interface/changeUserMsg.php";
                thread {
                    try {
                        val client = OkHttpClient()
                        //val requestBody=FormBody.Builder().add("a","1").add("b","2").build()
                        val requestBody = jsonData.toRequestBody();
                        val request = Request.Builder()
                            .url(serverURL)
                            .post(requestBody)
                            .build()
                        val call=client.newCall(request)
                        val response = call.execute()//execute方法会阻塞在这里，必须等到服务器响应，得到response才会执行下面的代码
                        val requestData = response.body?.string();
                        val resultJSON = JSONObject(requestData);
                        if (requestData != null) {
                            Log.e("postSync", requestData)
                            val mystatus = resultJSON["status"]
                            if(mystatus!="success"){
                                nameET.setText(oldname)
                            }
                        }
                    } catch (e: Exception) {
                        e.printStackTrace()
                    }
//                    runOnUiThread {
//                        nameTV.setText(userName);
//                        // 在这里更新你的adapter和UI
//                        adapter.notifyDataSetChanged();
//
//                    }
                }
            }


        }

        //del course
        delBtn.setOnClickListener {
            addordelcourse(currentCourseID,"deletecourse");
        }
        addBtn.setOnClickListener {
            val builder = AlertDialog.Builder(this)
            builder.setTitle("请输入选课码")
            val input = EditText(this)
            input.setHint("选课码")
            builder.setView(input)
            builder.setPositiveButton("确定") { dialog, _ ->
                val courseCode = input.text.toString();
                addordelcourse(courseCode,"addcourse");
            }
            builder.setNegativeButton("取消") { dialog, _ ->
                dialog.cancel()
            }
            builder.create().show()
        }
        //class begin
        classBtn.setOnClickListener {
            //判断是否已选择课程
            //判断是否正在上课
            val intent = Intent(this, CyberClass::class.java)
            intent.putExtra("userID", userID);
            intent.putExtra("courseID", currentCourseID);
            intent.putExtra("courseName",currentCourseName );
            startActivity(intent)

            //maybe just hide
            finish();
        }

        //enter class
        enterBtn.setOnClickListener {
            val intent = Intent(this, inClass::class.java)
            intent.putExtra("userID", userID);
            intent.putExtra("courseID", currentCourseID);
            intent.putExtra("courseName",currentCourseName );
            startActivity(intent)
            finish()
        }
    }

    //get user msg & his courselist for mainwindow
    private fun getMSG(){
    //private fun getMSG(){
        Log.e("hello","hello");
        //network module
        Log.e("userID: ",userID);
        val jsonData = """  
            {  
                "userID":"$userID",
                "password": "nothing"  
            }  
            """
        val serverURL = "http://43.138.243.228/cyberpodium/And_interface/MainWindMsg.php";
        val nameTV:EditText = findViewById(R.id.nicknameEditText);
        thread {
            try {
                val client = OkHttpClient()
                //val requestBody=FormBody.Builder().add("a","1").add("b","2").build()
                val requestBody = jsonData.toRequestBody();
                val request = Request.Builder()
                    .url(serverURL)
                    .post(requestBody)
                    .build()
                val call=client.newCall(request)
                val response = call.execute()//execute方法会阻塞在这里，必须等到服务器响应，得到response才会执行下面的代码
                val requestData = response.body?.string();
                val resultJSON = JSONObject(requestData);
                if (requestData != null) {
                    Log.e("postSync", requestData)
                    val mystatus = resultJSON["status"];
                    val mystatus2 = resultJSON["status2"];
                    if(mystatus=="success"&&mystatus2=="success"){
                        userName = resultJSON["username"].toString();
                        Log.e("username",userName);

                        courseshow.clear();
                        courseList.clear();
                        var index = 0;
                        while(true){
                            if(resultJSON[index.toString()]!=null){
                                val courseInfo = resultJSON[index.toString()];
                                Log.e("here",index.toString());
                                lateinit var item: courseJsonItem
                                item = courseJsonItem("","","","","","");

                                if(courseInfo is JSONObject){
                                    item.courseID = courseInfo["0"].toString();
                                    item.courseName = courseInfo["1"].toString();
                                    item.time = courseInfo["2"].toString();
                                    item.place = courseInfo["3"].toString();
                                    item.remark = courseInfo["4"].toString();
                                    var str:String = item.courseName;
                                    str += " ";str += item.time;
                                    str += " ";str += item.place;
                                    str += " ";str += item.builder;
                                    str += " ";str += item.remark;
                                    courseshow.add(str);
                                    courseList.add(item);

                                }



                            }else{
                                Log.e("index",index.toString());
                                break;
                            }
                            index++;

                        }
                    }

                }

            } catch (e: Exception) {
                e.printStackTrace()
            }
            runOnUiThread {
                nameTV.setText(userName);
                // 在这里更新你的adapter和UI
                adapter.notifyDataSetChanged();

            }
        }
        Log.e("?","?");

    }

    private fun addordelcourse(cid:String,opt:String){
        var thoastmsg = "退课成功";
        var dogetmsg = false;
        if(opt=="addcourse"){
            thoastmsg = "加课成功";
            //show dialect

        }
        val jsonData = """  
            {  
                "userID":"$userID",
                "courseID": "$cid",
                "option":"$opt"  
            }  
            """
        val serverURL = "http://43.138.243.228/cyberpodium/And_interface/changeCourse.php";
        thread {
            try {
                val client = OkHttpClient()
                //val requestBody=FormBody.Builder().add("a","1").add("b","2").build()
                val requestBody = jsonData.toRequestBody();
                val request = Request.Builder()
                    .url(serverURL)
                    .post(requestBody)
                    .build()
                val call=client.newCall(request)
                val response = call.execute()//execute方法会阻塞在这里，必须等到服务器响应，得到response才会执行下面的代码
                val requestData = response.body?.string();
                val resultJSON = JSONObject(requestData);
                if (requestData != null) {
                    Log.e("postSync", requestData)
                    val mystatus = resultJSON["status"];
                    val mymsg = resultJSON["message"];
                    if(mystatus == "success"){
                        dogetmsg=true;
                    }else if(mystatus == "failure"){
                        //no such course add course failure
                        thoastmsg= "加课失败！无效的课程id";
                    }else{
                        thoastmsg = "服务器出错";
                        //sql error
                    }

                }
                runOnUiThread {
                    Toast.makeText(this, "$thoastmsg", Toast.LENGTH_SHORT).show();
                    if(dogetmsg){
                        getMSG();
                        adapter.notifyDataSetChanged();
                    }
                }


            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

}

