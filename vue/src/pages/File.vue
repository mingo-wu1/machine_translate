<template>
	<Container class="mt-5">
		<Row>
			<div class="col-12 col-md-12">
				<div class="card">

					<div class="card-header">
						<h2>Send File</h2>
					</div>
                    
					<div class="card-body">
						<table class="table table-hover" v-if="savedTranslation.length">
							<thead>
								<th>#</th>
								<th>Original Text</th>
								<th>Translated Text</th>
								<th></th>
							</thead>
							<tbody>
								<tr v-for="(text, index) in savedTranslation" :key="text.id">
									<td>{{index+1}}</td>
									<td>{{text.origin}}</td>
									<td>{{text.result}}</td>
									<td>
										<button class="btn btn-sm btn-danger" @click.prevent="deleteData(text.id)">Delete</button>
									</td>
								</tr>
							</tbody>
						</table>
                        <div v-else>
                            <center><h3>No file translation, maybe send one?</h3></center>
                        </div>
					</div>

				</div>

                <div>
                    <center><button @click="onOpenButtonClick">websocket连接/发送</button></center>
                    <center><button @click="onCloseButtonClick">websocket关闭</button></center>
                </div>

			</div>
		</Row>
	</Container>
</template>

<script>
import Container from "../components/Container";
import Row from "../components/Row";
export default {
	components: {
		Container,
		Row,
	},
	data() {
		return {
			savedTranslation: [],

            // web socket
            path:"ws://127.0.0.1:10002",
            socket:"",
		};
	},
	beforeRouteEnter(to, from, next) {
		next((vm) => {
			vm.fetchData(next);
		});
	},
	methods: {
		fetchData() {
			if (localStorage.getItem("savedTranslation")) {
				this.savedTranslation = JSON.parse(
					localStorage.getItem("savedTranslation")
				);
			}
		},
		deleteData(id) {
			let self = this;
			this.$swal({
				title: "Do you want to delete this data?",
				showDenyButton: true,
				showCancelButton: true,
				confirmButtonText: `Delete`,
				denyButtonText: `Don't save`,
			}).then((result) => {
				if (result.isConfirmed) {
					this.savedTranslation = this.savedTranslation.filter(
						(sv) => sv.id !== id
					);
					localStorage.setItem(
						"savedTranslation",
						JSON.stringify(this.savedTranslation)
					);
					self.$swal("Deleted!", "", "success");
				}
			});
		},

        // web socket
        onOpenButtonClick(){
            var result = this.init()
            if (1 == result) {
                console.log("web socket init success")
                this.send()
                console.log("web socket send success")
            } else {
                console.log("web socket init fail")
            }          
        },
        onCloseButtonClick(){
            this.close()
            console.log("web socket close success")
        },
        init: function () {
            if(typeof(WebSocket) === "undefined"){
                alert("您的浏览器不支持socket")
                return 0
            }else{
                // 实例化socket
                this.socket = new WebSocket(this.path)
                // 监听socket连接
                this.socket.onopen = this.open
                // 监听socket错误信息
                this.socket.onerror = this.error
                // 监听socket消息
                this.socket.onmessage = this.getMessage
                return 1
            }
        },
        open: function () {
            console.log("socket连接成功")
        },
        error: function () {
            console.log("连接错误")
        },
        getMessage: function (msg) {
            console.log(msg.data)
        },
        send: function () {
            // this.socket.send(params)
            console.log("sending")
        },
        close: function () {
            console.log("socket已经关闭")
        },
	},
};
</script>