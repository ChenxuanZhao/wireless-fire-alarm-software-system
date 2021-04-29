<template>
	<view>
		<view class="uni-padding-wrap uni-common-mt">
			<form @submit="formSubmit" @reset="formReset">
				<view class="uni-form-item uni-column">
					<view class="title">温度阈值 (单位: ℃)</view>
					<slider value="60" min="-55" max="125" name="temperatureThreshold" show-value></slider>
				</view>
				<view class="uni-form-item uni-column">
					<view class="title">浓度阈值 (单位: ppm)</view>
					<slider value="300" min="200" max="10000" name="concentrationThreshold" show-value></slider>
				</view>
				<view class="btn_view">
					<button :loading="isLoading" type="primary" form-type="submit">提交</button>
				</view>
				<view class="btn_view">
					<button type="warn" form-type="reset">重置</button>
				</view>
			</form>
		</view>
	</view>
</template>
<script>
	export default {
		data() {
			return {
				tips: "",
				isLoading: false
			}
		},
		methods: {
			postServerData(e) {
				let that = this
				this.isLoading = true
				uni.request({
					url: 'http://172.20.10.2/api/v1/config/1/',
					// url: 'https://design.xuanxuan.cf/api/v1/config/1/',
					data: e,
					method: 'PUT',
					success: function(res) {
						console.log("OK")
						that.isLoading = false
					},
					fail: () => {
						tips = "网络错误，小程序端请检查合法域名"
						that.isLoading = false
					}
				})
			},
			formSubmit: function(e) {
				var formData = JSON.stringify(e.detail.value)
				console.log('form发生了submit事件，携带数据为：' + formData)
				this.postServerData(formData)
			},
			formReset: function(e) {
				console.log('清空数据')
			}
		}
	}
</script>

<style>
	.uni-form-item .title {
		padding: 20rpx 0;
	}

	.btn_view {
		margin: 15px auto;
		width: 250px;
		height: 40px;
		line-height: 40px;
		text-align: center;
		border-radius: 5px;
	}
</style>
