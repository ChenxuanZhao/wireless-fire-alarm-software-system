<template>
	<view>
		<uni-notice-bar v-if="isDisplayNoteMessage" showClose="true" showIcon="true" :text="noteMessage"></uni-notice-bar>
		<view class="qiun-columns">
			<view class="qiun-charts">
				<!--#ifdef MP-ALIPAY -->
				<canvas canvas-id="canvasLineA" id="canvasLineA" class="charts" :width="cWidth*pixelRatio" :height="cHeight*pixelRatio"
				 :style="{'width':cWidth+'px','height':cHeight+'px'}" @touchstart="touchLineA" @touchmove="moveLineA" @touchend="touchEndLineA"></canvas>
				<!--#endif-->
				<!--#ifndef MP-ALIPAY -->
				<canvas canvas-id="canvasLineA" id="canvasLineA" class="charts" @touchstart="touchLineA" @touchmove="moveLineA"
				 @touchend="touchEndLineA"></canvas>
				<!--#endif-->
			</view>
		</view>
		<view class="qiun-columns">
			<view class="qiun-charts">
				<!--#ifdef MP-ALIPAY -->
				<canvas canvas-id="canvasLineB" id="canvasLineB" class="charts" :width="cWidth*pixelRatio" :height="cHeight*pixelRatio"
				 :style="{'width':cWidth+'px','height':cHeight+'px'}" @touchstart="touchLineB" @touchmove="moveLineB" @touchend="touchEndLineB"></canvas>
				<!--#endif-->
				<!--#ifndef MP-ALIPAY -->
				<canvas canvas-id="canvasLineB" id="canvasLineB" class="charts" @touchstart="touchLineB" @touchmove="moveLineB"
				 @touchend="touchEndLineB"></canvas>
				<!--#endif-->
			</view>
		</view>
		<view class="btn_view">
			<button :loading="isLoading" @click="getServerData" type="primary" size="mini">更新数据</button>
		</view>
	</view>
</template>

<script>
	import uCharts from '@/components/u-charts/u-charts.js'
	import uniNoticeBar from '@/components/uni-notice-bar/uni-notice-bar.vue'

	var _self
	var canvaLineA = null
	var canvaLineB = null
	var lastMoveTime = null //最后执行移动的时间戳
	var timerUpdate = null

	export default {
		components: {
			uniNoticeBar
		},
		data() {
			return {
				cWidth: '',
				cHeight: '',
				pixelRatio: 1,
				InteractiveA: '', //交互显示的数据
				InteractiveB: '', //交互显示的数据
				temperature: 60,
				concentration: 2000,
				temperatureThreshold: 60,
				concentrationThreshold: 2000,
				noteMessage: '',
				isDisplayNoteMessage: false,
				isLoading: false,
				isInit: false,
			}
		},
		onLoad() {
			_self = this;
			//#ifdef MP-ALIPAY
			uni.getSystemInfo({
				success: function(res) {
					if (res.pixelRatio > 1) {
						_self.pixelRatio = 2
					}
				}
			});
			//#endif
			this.cWidth = uni.upx2px(750)
			this.cHeight = uni.upx2px(500)
			timerUpdate = setInterval(callback => {
				this.getServerData()
			}, 1000 * 10)
			this.getServerData()
		},
		onShow() {
			this.getServerData()
		},
		onUnload() {
			clearInterval(timerUpdate)
			clearInterval(timerUpdate1)
		},
		methods: {
			toJSON() {},
			getServerData() {
				let that = this
				this.isLoading = true
				uni.request({
					url: 'http://172.20.10.2/api/v1/config/',
					// url: 'https://design.xuanxuan.cf/api/v1/config/',
					data: {},
					success: function(res) {
						console.log(res)
						let results = res.data.results[0]
						that.temperatureThreshold = results.temperatureThreshold;
						that.concentrationThreshold = results.concentrationThreshold;
					},
					fail: () => {
						_self.tips = "网络错误，小程序端请检查合法域名"
					},
				})
				uni.request({
					url: 'http://172.20.10.2/api/v1/data/?ordering=-updateTime',
					// url: 'https://design.xuanxuan.cf/api/v1/data/?ordering=-updateTime',
					data: {},
					method: 'GET',
					success: function(res) {
						let results = res.data.results
						let LineA = {
							categories: ["", "", "", "", ""],
							series: [{
								name: '温度',
								data: [0, 0, 0, 0, 0],
							}]
						}
						let LineB = {
							categories: ["", "", "", "", ""],
							series: [{
								name: '浓度',
								data: [0, 0, 0, 0, 0],
							}]
						}
						that.temperature = results[0].temperature
						that.concentration = results[0].smokescope
						for (let i = 0; i < 5; i++) {
							LineA.categories[4 - i] += results[i].updateTime
							LineA.series[0].data[4 - i] += results[i].temperature
							LineB.categories[4 - i] += results[i].updateTime
							LineB.series[0].data[4 - i] += results[i].smokescope
						}
						if (that.isInit) {
							canvaLineA.updateData(LineA);
							canvaLineB.updateData(LineB);
						} else {
							_self.showLineA("canvasLineA", LineA)
							_self.showLineB("canvasLineB", LineB)
						}
						that.isInit = true

						that.isLoading = false

						if (that.temperature > that.temperatureThreshold || that.concentration > that.concentrationThreshold) {
							if (that.temperature > that.temperatureThreshold) {
								that.noteMessage = '温度异常'
							}
							if (that.concentration > that.concentrationThreshold) {
								that.noteMessage = '烟雾浓度异常'
							}
							that.isDisplayNoteMessage = true
						} else {
							that.noteMessage = ''
							that.isDisplayNoteMessage = false
						}
					},
					fail: () => {
						_self.tips = "网络错误，小程序端请检查合法域名"
						that.isLoading = false
					},
				})
			},
			showLineA(canvasId, chartData) {
				canvaLineA = new uCharts({
					$this: _self,
					canvasId: canvasId,
					type: 'line',
					fontSize: 11,
					legend: {
						show: true
					},
					dataLabel: true,
					dataPointShape: true,
					background: '#FFFFFF',
					pixelRatio: _self.pixelRatio,
					categories: chartData.categories,
					series: chartData.series,
					animation: false,
					xAxis: {
						type: 'grid',
						gridColor: '#CCCCCC',
						gridType: 'dash',
						dashLength: 8,
						disabled: true
					},
					yAxis: {
						gridType: 'dash',
						gridColor: '#CCCCCC',
						dashLength: 8,
						splitNumber: 5,
						min: -55,
						max: 125,
						format: (val) => {
							return val.toFixed(0) + '℃'
						}
					},
					width: _self.cWidth * _self.pixelRatio,
					height: _self.cHeight * _self.pixelRatio,
					extra: {
						line: {
							type: 'straight'
						}
					}
				})
			},
			touchLineA(e) {
				lastMoveTime = Date.now()
			},
			moveLineA(e) {
				let currMoveTime = Date.now()
				let duration = currMoveTime - lastMoveTime
				if (duration < Math.floor(1000 / 60)) {
					return
				}
				lastMoveTime = currMoveTime

				let currIndex = canvaLineA.getCurrentDataIndex(e)
				if (currIndex > -1 && currIndex < canvaLineA.opts.categories.length) {
					let riqi = canvaLineA.opts.categories[currIndex]
					let leibie = canvaLineA.opts.series[0].name
					let shuju = canvaLineA.opts.series[0].data[currIndex]
					this.InteractiveA = leibie + ':' + riqi + ' ' + shuju + '℃'
				}
				canvaLineA.showToolTip(e, {
					format: function(item, category) {
						return category + ' ' + item.name + ':' + item.data + '℃'
					}
				});
			},
			touchEndLineA(e) {
				let currIndex = canvaLineA.getCurrentDataIndex(e)
				if (currIndex > -1 && currIndex < canvaLineA.opts.categories.length) {
					let riqi = canvaLineA.opts.categories[currIndex]
					let leibie = canvaLineA.opts.series[0].name
					let shuju = canvaLineA.opts.series[0].data[currIndex]
					this.InteractiveA = leibie + ' ' + ':' + riqi + ' ' + shuju + '℃'
				}
				canvaLineA.touchLegend(e)
				canvaLineA.showToolTip(e, {
					format: function(item, category) {
						return category + ' ' + item.name + ':' + item.data + '℃'
					}
				})
			},

			showLineB(canvasId, chartData) {
				canvaLineB = new uCharts({
					$this: _self,
					canvasId: canvasId,
					type: 'line',
					fontSize: 11,
					legend: {
						show: true
					},
					dataLabel: true,
					dataPointShape: true,
					background: '#FFFFFF',
					pixelRatio: _self.pixelRatio,
					categories: chartData.categories,
					series: chartData.series,
					animation: false,
					xAxis: {
						type: 'grid',
						gridColor: '#CCCCCC',
						gridType: 'dash',
						dashLength: 8,
						disabled: true
					},
					yAxis: {
						gridType: 'dash',
						gridColor: '#CCCCCC',
						dashLength: 8,
						splitNumber: 5,
						min: 200,
						max: 10000,
						format: (val) => {
							return val.toFixed(0) + 'ppm'
						}
					},
					width: _self.cWidth * _self.pixelRatio,
					height: _self.cHeight * _self.pixelRatio,
					extra: {
						line: {
							type: 'straight'
						}
					}
				});
			},
			touchLineB(e) {
				lastMoveTime = Date.now()
			},
			moveLineB(e) {
				let currMoveTime = Date.now()
				let duration = currMoveTime - lastMoveTime
				if (duration < Math.floor(1000 / 60)) return; //每秒60帧
				lastMoveTime = currMoveTime

				let currIndex = canvaLineB.getCurrentDataIndex(e)
				if (currIndex > -1 && currIndex < canvaLineB.opts.categories.length) {
					let riqi = canvaLineB.opts.categories[currIndex]
					let leibie = canvaLineB.opts.series[0].name
					let shuju = canvaLineB.opts.series[0].data[currIndex]
					this.InteractiveB = leibie + ':' + riqi + ' ' + shuju + 'ppm'
				}
				canvaLineB.showToolTip(e, {
					format: function(item, category) {
						return category + ' ' + item.name + ':' + item.data + 'ppm'
					}
				})
			},
			touchEndLineB(e) {
				let currIndex = canvaLineB.getCurrentDataIndex(e)
				if (currIndex > -1 && currIndex < canvaLineB.opts.categories.length) {
					let riqi = canvaLineB.opts.categories[currIndex]
					let leibie = canvaLineB.opts.series[0].name
					let shuju = canvaLineB.opts.series[0].data[currIndex]
					this.InteractiveB = leibie + ' ' + ':' + riqi + ' ' + shuju + 'ppm'
				}
				canvaLineB.touchLegend(e)
				canvaLineB.showToolTip(e, {
					format: function(item, category) {
						return category + ' ' + item.name + ':' + item.data + 'ppm'
					}
				})
			},
		}
	}
</script>

<style>
	/*样式的width和height一定要与定义的cWidth和cHeight相对应*/
	.qiun-charts {
		width: 750upx;
		height: 500upx;
		background-color: #FFFFFF;
	}

	.charts {
		width: 750upx;
		height: 500upx;
		background-color: #FFFFFF;
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
